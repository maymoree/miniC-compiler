#include <llvm-c/Core.h>
#include <ast.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstring>
#include <algorithm>
#include <cassert>

using namespace std;

LLVMModuleRef ir_builder(astNode* root);
void pre_traverse(astNode* node, vector<vector<string>*>* stack_variable_names);
LLVMModuleRef build_traverse(astNode* node, LLVMModuleRef mod);
LLVMBasicBlockRef genIRStmt(astStmt* node, LLVMBuilderRef &builder, LLVMBasicBlockRef startBB, map<string, LLVMValueRef>* var_map, LLVMModuleRef mod, LLVMValueRef func);
LLVMValueRef genIRExpr(astNode* node, LLVMBuilderRef builder, map<string, LLVMValueRef>* var_map, LLVMModuleRef mod);
map<LLVMBasicBlockRef, set<LLVMBasicBlockRef>> pred_map(LLVMValueRef func);
void clean_module(map<LLVMBasicBlockRef, set<LLVMBasicBlockRef>> preds, LLVMValueRef func);

bool bod_of_func;
map<string, int>* names_tracker_map = new map<string, int> ();
LLVMValueRef ret_ref;
LLVMBasicBlockRef retBB;

LLVMModuleRef ir_builder(astNode* root) {

    vector<vector<string>*>* stack_variable_names = new vector<vector<string>*>(); // holds all variable names

    pre_traverse(root, stack_variable_names);

    printNode(root);

    LLVMModuleRef mod = build_traverse(root, NULL);

    LLVMDumpModule(mod);

    LLVMPrintModuleToFile(mod, "pt2.ll", NULL);

    // free and clean up
    delete(names_tracker_map);
    for (auto vec : *stack_variable_names) {
        delete(vec);
    }
    delete(stack_variable_names);

    return mod;
}


LLVMModuleRef build_traverse(astNode* node, LLVMModuleRef mod) {

    node_type type = node->type;

    if (type == ast_prog) {
        // create module & set target
        mod = LLVMModuleCreateWithName("");
        LLVMSetTarget(mod, "x86_64-pc-linux-gnu");

        // create extern print --> func without bodies
        LLVMTypeRef param_types_print[] = {LLVMInt32Type()};
        LLVMTypeRef print_type = LLVMFunctionType(LLVMVoidType(), param_types_print, 1, 0);
        LLVMValueRef print = LLVMAddFunction(mod, "print", print_type);

        // create extern read --> func without bodies
        LLVMTypeRef param_types_read[] = {};
        LLVMTypeRef read_type = LLVMFunctionType(LLVMInt32Type(), param_types_read, 0, 0);
        LLVMValueRef read = LLVMAddFunction(mod, "read", read_type);

        // visit the func
        build_traverse(node->prog.func, mod);
    }

    else if (type == ast_func) {
        
        // generate builder
        LLVMBuilderRef builder = LLVMCreateBuilder();

        // check for param (either 1 or 0)
        LLVMTypeRef param_types[] = {};
        int num_param = 0;
        if (node->func.param != NULL) {
            param_types[0] = {LLVMInt32Type()};
            num_param = 1;
        }

        // create func type first to add entryBB
        LLVMTypeRef func_type = LLVMFunctionType(LLVMInt32Type(), param_types, num_param, 0);
        LLVMValueRef func = LLVMAddFunction(mod, node->func.name, func_type);

        // generate first basic block
        LLVMBasicBlockRef entryBB = LLVMAppendBasicBlock(func, ""); // 

        // create a set of all parameters and variables
        set<string> names_set;
        // add parameter into set if it exists
        if (num_param == 1) {
            names_set.insert(node->func.param->var.name);
        }
        // have to add in variable names into set
        auto itr = names_tracker_map->begin(); 
        while (itr != names_tracker_map->end()) { 
            // key name
            string name = itr->first; 
            // value num
            int count = itr->second; 
            // add first name into set (e.g. "a")
            names_set.insert(name);
            // if there's more than 1 a (e.g. "a1" "a2") --> shown by counts
            if (count != 0) {
                // loop through each count and add it to set
                for (int i = 1; i <= count; i++){
                    string combined_name = name + to_string(i);
                    names_set.insert(combined_name);
                }
            }
            // iterator incremented to point next item 
            itr++; 
        } 

        // set<string>::iterator it;
        // for(it = names_set.begin(); it != names_set.end(); it++){
        //     printf("%s\n", it->c_str());
        // }

        // move builder position to end of entry BB
        LLVMPositionBuilderAtEnd(builder, entryBB);

        // generate var map and append name,alloc for names in set
        map<string, LLVMValueRef>* var_map = new map<string, LLVMValueRef> ();
        set<string>::iterator name;
        for (name = names_set.begin(); name != names_set.end(); ++name){
            LLVMValueRef alloc = LLVMBuildAlloca(builder, LLVMInt32Type(), name->c_str()); // curently only param for now
            LLVMSetAlignment(alloc, 4);
            var_map->insert(pair<string, LLVMValueRef>(*name, alloc));
        }

        // auto it2 = var_map->begin(); 
        // while (it2 != var_map->end()) { 
        //     printf("\nin map: %s ", it2->first.c_str());
        //     LLVMDumpValue(it2->second);
        //     printf("\n");
        //     // iterator incremented to point next item 
        //     it2++; 
        // } 

        // alloca for ret_ref --> global variable
        ret_ref = LLVMBuildAlloca(builder, LLVMInt32Type(), "ret_ref");
        LLVMSetAlignment(ret_ref, 4);

        // store for func param
        if (num_param == 1) {
            LLVMBuildStore(builder, LLVMGetParam(func, 0), var_map->at(node->func.param->var.name)); // LLVMGetParam(function, index of param)
        }

        // return BB --> global variable
        retBB = LLVMAppendBasicBlock(func, "");

        // move builder position to end of retBB
        LLVMPositionBuilderAtEnd(builder, retBB);

        // add load and return to retBB
        LLVMValueRef load = LLVMBuildLoad2(builder, LLVMInt32Type(), ret_ref, "");
        LLVMBuildRet(builder, load);
       
        LLVMBasicBlockRef exitBB = genIRStmt(&node->func.body->stmt, builder, entryBB, var_map, mod, func);

        // get terminator instruc of exitBB and check if NULL
        if (LLVMGetBasicBlockTerminator(exitBB) == NULL) { // no return statement
            LLVMPositionBuilderAtEnd(builder, exitBB);
            LLVMBuildBr(builder, retBB);
        }

        map<LLVMBasicBlockRef, set<LLVMBasicBlockRef>> preds = pred_map(func);
        clean_module(preds, func);

        /*
        Time for memory cleanup (delete memory from maps, sets, builder)!
        */

        var_map->clear();
        delete(var_map);
        LLVMDisposeBuilder(builder);

        return mod;

    }

    return mod;
    
}

LLVMBasicBlockRef genIRStmt(astStmt* node, LLVMBuilderRef &builder, LLVMBasicBlockRef startBB, map<string, LLVMValueRef>* var_map, LLVMModuleRef mod, LLVMValueRef func) {

    assert(node != NULL && builder != NULL);
    switch (node->type) {
        case ast_asgn: {
            // set position at end of startBB
            LLVMPositionBuilderAtEnd(builder, startBB);
            LLVMValueRef rhs = genIRExpr(node->asgn.rhs, builder, var_map, mod); 
            LLVMValueRef store_val = LLVMBuildStore(builder, rhs, var_map->at(node->asgn.lhs->var.name));
            return startBB;
        }
        case ast_call: {
            // must be a print stmt... no checks since grammar should be correct

            // set position at end of startBB
            LLVMPositionBuilderAtEnd(builder, startBB);
            
            // generate val of printed
            LLVMValueRef print_val = genIRExpr(node->call.param, builder, var_map, mod);

            // generate print type --> have to check similar to read!!!!! ------------------------------------
            LLVMTypeRef param_types_print[] = {LLVMInt32Type()};
            LLVMTypeRef print_type = LLVMFunctionType(LLVMVoidType(), param_types_print, 1, 0);

            // generate function from module
            LLVMValueRef print_func = LLVMGetNamedFunction(mod, "print");

            // generate llvmvalueref* args
            LLVMValueRef* param = (LLVMValueRef*)malloc(sizeof(LLVMValueRef));
            param[0] = print_val;

            LLVMValueRef call_val = LLVMBuildCall2(builder, print_type, print_func, param, 1, "");
            free(param);
            return startBB;
        }
        case ast_while: {

            // set position at end of startBB
            LLVMPositionBuilderAtEnd(builder, startBB);

            // gen basic block --> entry for while loop
            LLVMBasicBlockRef condBB = LLVMAppendBasicBlock(func, "");

            // gen branch at end of startBB
            LLVMBuildBr(builder, condBB); 

            // set pos builder to end of condBB
            LLVMPositionBuilderAtEnd(builder, condBB);

            // genirexpr for condition
            LLVMValueRef cond_val = genIRExpr(node->whilen.cond, builder, var_map, mod);

            // create 2 blocks and conditional branch
            LLVMBasicBlockRef trueBB = LLVMAppendBasicBlock(func, "");
            LLVMBasicBlockRef falseBB = LLVMAppendBasicBlock(func, "");
            LLVMBuildCondBr(builder, cond_val, trueBB, falseBB);

            // gen trueExitBB
            LLVMBasicBlockRef trueExitBB = genIRStmt(&node->whilen.body->stmt, builder, trueBB, var_map, mod, func);

            // set pos builder to end of trueExitBB
            LLVMPositionBuilderAtEnd(builder, trueExitBB);

            // unconditional branch --> has to link to LLVMBuilderRef, but links to trueExitBB it's moved to end of builder right???
            LLVMBuildBr(builder, condBB);


            // return falseBB
            return falseBB;
        }
        case ast_if: {
            // set pos to end of startBB
            LLVMPositionBuilderAtEnd(builder, startBB);
            
            // genirexpr for condition
            LLVMValueRef cond_val = genIRExpr(node->ifn.cond, builder, var_map, mod);

            // create 2 blocks and conditional branch
            LLVMBasicBlockRef trueBB = LLVMAppendBasicBlock(func, "");
            LLVMBasicBlockRef falseBB = LLVMAppendBasicBlock(func, "");
            LLVMBuildCondBr(builder, cond_val, trueBB, falseBB);

            if (node->ifn.else_body == NULL) { // no else
                LLVMBasicBlockRef ifExitBB = genIRStmt(&node->ifn.if_body->stmt, builder, trueBB, var_map, mod, func);
                LLVMPositionBuilderAtEnd(builder, ifExitBB);
                LLVMBuildBr(builder, falseBB);
                return falseBB;
            } // else body
            LLVMBasicBlockRef ifExitBB = genIRStmt(&node->ifn.if_body->stmt, builder, trueBB, var_map, mod, func);
            LLVMBasicBlockRef elseExitBB = genIRStmt(&node->ifn.else_body->stmt, builder, falseBB, var_map, mod, func);
            LLVMBasicBlockRef endBB = LLVMAppendBasicBlock(func, "");
            LLVMPositionBuilderAtEnd(builder, ifExitBB);
            LLVMBuildBr(builder, endBB);
            LLVMPositionBuilderAtEnd(builder, elseExitBB);
            LLVMBuildBr(builder, endBB);
            return endBB;
        }
        case ast_ret: {
            LLVMPositionBuilderAtEnd(builder, startBB);

            LLVMValueRef ret_val = genIRExpr(node->ret.expr, builder, var_map, mod);

            LLVMBuildStore(builder, ret_val, ret_ref);
            LLVMBuildBr(builder, retBB);
            LLVMBasicBlockRef endBB = LLVMAppendBasicBlock(func, "");
            return endBB;
        }
        case ast_block: {
            LLVMBasicBlockRef prevBB = startBB;
            for (int s = 0; s < node->block.stmt_list->size(); s++){
                prevBB = genIRStmt(&(*node->block.stmt_list->at(s)).stmt, builder, prevBB, var_map, mod, func);
            }

            LLVMBasicBlockRef endBB = prevBB;
            return endBB;
        }
        default: {return startBB;}
    }

}

LLVMValueRef genIRExpr(astNode* node, LLVMBuilderRef builder, map<string, LLVMValueRef>* var_map, LLVMModuleRef mod) {

    switch(node->type) {
        case ast_cnst: {
            // turn node->cnst.value (int) to llvmvalueref; extend sign or not?
            LLVMValueRef cnst_val = LLVMConstInt(LLVMInt32Type(), node->cnst.value, 1);
            return cnst_val;
        }
        case ast_var: {
            LLVMValueRef var_alloc = var_map->at(node->var.name);
            LLVMValueRef var_val = LLVMBuildLoad2(builder, LLVMInt32Type(), var_alloc, "");
            return var_val;
        }
        case ast_uexpr: {
            LLVMValueRef expr = genIRExpr(node->uexpr.expr, builder, var_map, mod);
            // turn int to llvmvalueref
            LLVMValueRef const_zero = LLVMConstInt(LLVMInt32Type(), 0, 1);
            LLVMValueRef sub_val = LLVMBuildSub(builder, const_zero, expr, "");
            return sub_val; 
        }
        case ast_bexpr: {
            LLVMValueRef lhs = genIRExpr(node->bexpr.lhs, builder, var_map, mod);
            LLVMValueRef rhs = genIRExpr(node->bexpr.rhs, builder, var_map, mod);
            op_type op = node->bexpr.op;
            if (op == add) {
                return LLVMBuildAdd(builder, lhs, rhs, "");
            } 
            else if (op == sub) {
                return LLVMBuildSub(builder, lhs, rhs, "");
            }
            else if (op == mul) {
                return LLVMBuildMul(builder, lhs, rhs, "");
            }
            else if (op == divide) {
                return LLVMBuildSDiv(builder, lhs, rhs, ""); // not in instructions but in case?????
            }
        }
        case ast_rexpr: {
            LLVMValueRef lhs = genIRExpr(node->rexpr.lhs, builder, var_map, mod);
            LLVMValueRef rhs = genIRExpr(node->rexpr.rhs, builder, var_map, mod);
            rop_type op = node->rexpr.op;
            if (op == lt) {
                return LLVMBuildICmp(builder, LLVMIntSLT, lhs, rhs, "");
            } 
            else if (op == gt) {
                return LLVMBuildICmp(builder, LLVMIntSGT, lhs, rhs, "");
            }
            else if (op == le) {
                return LLVMBuildICmp(builder, LLVMIntSLE, lhs, rhs, "");
            }
            else if (op == ge) {
                return LLVMBuildICmp(builder, LLVMIntSGE, lhs, rhs, "");
            }
            else if (op == eq) {
                return LLVMBuildICmp(builder, LLVMIntEQ, lhs, rhs, "");
            }
            else if (op == neq) {
                return LLVMBuildICmp(builder, LLVMIntNE, lhs, rhs, "");
            }
        }
        case ast_stmt: {
            // must be a read function to assign it something!                   
            LLVMValueRef read_func = LLVMGetNamedFunction(mod, "read");

            LLVMTypeRef param_types_read[] = {};
            LLVMTypeRef read_type = LLVMFunctionType(LLVMInt32Type(), param_types_read, 0, 0);
            LLVMValueRef call_val = LLVMBuildCall2(builder, read_type, read_func, NULL, 0, "");
            return call_val;
        }
        default: {return NULL;}
    }

}





// PRE PROCESSING !!!!!

void pre_traverse(astNode* node, vector<vector<string>*>* stack_variable_names) {
    node_type type = node->type;

    // BLOCK AND FUNCTION BODY --> happens once
    if (type == ast_stmt && node->stmt.type == ast_block && bod_of_func){    
        vector<string>* curr_sym_table = stack_variable_names->back();
        bod_of_func = false;
        for (size_t i = 0; i < node->stmt.block.stmt_list->size(); ++i){
            pre_traverse(node->stmt.block.stmt_list->at(i), stack_variable_names);
        }
    }

    // BLOCK
    else if (type == ast_stmt && node->stmt.type == ast_block){
        vector<string>* curr_var_names = new vector<string>();
        stack_variable_names->push_back(curr_var_names);
        
        for (size_t i = 0; i < node->stmt.block.stmt_list->size(); ++i){
            pre_traverse(node->stmt.block.stmt_list->at(i), stack_variable_names);
        }

        stack_variable_names->pop_back();
        delete curr_var_names;
    }

    // FUNCTION TYPE
    else if (type == ast_func){
        vector<string>* curr_var_names = new vector<string>();
        stack_variable_names->push_back(curr_var_names);

        if (node->func.param != NULL) {
            curr_var_names->push_back(node->func.param->var.name);
        }

        bod_of_func = true;
        pre_traverse(node->func.body, stack_variable_names);

        stack_variable_names->pop_back();
        delete curr_var_names;
    }

    // DECLARATION TYPE
    else if (type == ast_stmt && node->stmt.type == ast_decl){
        // printf("\n DECLARATION \n");

        string var_name = node->stmt.decl.name;
        int num_to_add = 0;

        vector<string>* curr_var_names = stack_variable_names->back();

        // find var_name in names_map
        auto itr = names_tracker_map->find(var_name);

        if (itr == names_tracker_map->end()) { // not found
            // printf("\nNOT FOUND\n");
            // add name to curr_var_names
            curr_var_names->push_back(var_name);
            names_tracker_map->insert(pair<string, int>(var_name, num_to_add)); // put in name with 0
        } else { // found
            // printf("\nFOUND\n");
            num_to_add = ++(itr->second); // increase the int tracker in map
            // change inputs to strings
            string num_to_add_str = to_string(num_to_add);
            string new_var_name_str = var_name + num_to_add_str;
            // update node name
            free(node->stmt.decl.name); // free the old name
            node->stmt.decl.name = strdup(new_var_name_str.c_str());
            // printf("\nname exists in all scopes! --> new name: %s\n", node->stmt.decl.name);
            // add new name to curr_var_names
            curr_var_names->push_back(new_var_name_str);
        }

        // for(auto it = names_tracker_map->begin(); it != names_tracker_map->end(); ++it) {
        //     printf("Map key: %s value: %d\n", it->first.c_str(), it->second);
        // }
    }

    // VARIABLE TYPE
    else if (type == ast_var) {
        string var_name = node->var.name;
        vector<string>* curr_var_names = stack_variable_names->back();

        for (size_t i = 0; i < curr_var_names->size(); ++i) {
            if (curr_var_names->at(i) == var_name) {
                free(node->var.name); // free the old name
                node->var.name = strdup(curr_var_names->at(i).c_str());
                // printf("\nfound name in scope! --> changed name: %s\n", node->var.name);
            }
        }
    }

    // EXTERN TYPE
    else if (type == ast_extern) {
        // handle extern type if necessary
    }

    // OTHER TYPES
    else if (type == ast_prog) {
        pre_traverse(node->prog.func, stack_variable_names);
    }

    else if (type == ast_cnst) {
        // handle constant type if necessary
    }

    else if (type == ast_rexpr) {
        // traverse left then right child
        pre_traverse(node->rexpr.lhs, stack_variable_names);
        pre_traverse(node->rexpr.rhs, stack_variable_names);
    }

    else if (type == ast_bexpr) {
        pre_traverse(node->bexpr.lhs, stack_variable_names);
        pre_traverse(node->bexpr.rhs, stack_variable_names);
    }

    else if (type == ast_uexpr) {
        pre_traverse(node->uexpr.expr, stack_variable_names);
    }

    else if (type == ast_stmt && node->stmt.type == ast_call) {
        if (node->stmt.call.param != NULL) {
            pre_traverse(node->stmt.call.param, stack_variable_names);
        }
    }

    else if (type == ast_stmt && node->stmt.type == ast_ret) {
        pre_traverse(node->stmt.ret.expr, stack_variable_names);
    }

    else if (type == ast_stmt && node->stmt.type == ast_while) {
        // two children
        pre_traverse(node->stmt.whilen.cond, stack_variable_names);
        pre_traverse(node->stmt.whilen.body, stack_variable_names);
    }

    else if (type == ast_stmt && node->stmt.type == ast_if) {
        // three children
        pre_traverse(node->stmt.ifn.cond, stack_variable_names);
        pre_traverse(node->stmt.ifn.if_body, stack_variable_names);

        // check else_body
        if (node->stmt.ifn.else_body != NULL) {
            pre_traverse(node->stmt.ifn.else_body, stack_variable_names);
        }
    }

    else if (type == ast_stmt && node->stmt.type == ast_asgn) {
        // two children
        pre_traverse(node->stmt.asgn.lhs, stack_variable_names);
        pre_traverse(node->stmt.asgn.rhs, stack_variable_names);
    }
}


// HELPER FUNCTIONS

// grab predecessors similarly to in optimization.c
map<LLVMBasicBlockRef, set<LLVMBasicBlockRef>> pred_map(LLVMValueRef func) {

    map<LLVMBasicBlockRef, set<LLVMBasicBlockRef>> preds;

    for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(func);
        basicBlock;
        basicBlock = LLVMGetNextBasicBlock(basicBlock)) {

            LLVMValueRef terminator = LLVMGetBasicBlockTerminator(basicBlock); // grab terminator

            if (terminator) { // if block has terminator

                // loop through all successors
                for (int i = 0; i < LLVMGetNumSuccessors(terminator); i++) {
                    // add basicblock into predecessor of successor
                    LLVMBasicBlockRef successor = LLVMGetSuccessor(terminator, i);
                    preds[successor].insert(basicBlock);
                }

            }

    }
	
    return preds;
				
}

void clean_module(map<LLVMBasicBlockRef, set<LLVMBasicBlockRef>> preds, LLVMValueRef func) {

    vector<LLVMBasicBlockRef> to_erase;

    LLVMBasicBlockRef firstBB = LLVMGetFirstBasicBlock(func);

    for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(func);
        basicBlock;
        basicBlock = LLVMGetNextBasicBlock(basicBlock)) {

        if (preds.find(basicBlock) == preds.end() && basicBlock != firstBB) {
            to_erase.push_back(basicBlock);
        }

    }

    for(int i = 0; i < to_erase.size(); i++){
        LLVMDeleteBasicBlock(to_erase.at(i));
    }

}