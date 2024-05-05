#include <ast.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

int semantic_analysis(astNode* root);
int traverse(vector<vector<char*>*> *stack_symbol_table, astNode* node, bool& passed);
bool body_of_func;

int semantic_analysis(astNode* root){

    vector<vector<char*>*>* stack_symbol_table = new vector<vector<char*>*> (); // holds all

    bool passed;
    passed = true; // starts as true

    if (root != NULL) {
        passed = traverse(stack_symbol_table, root, passed);
    }
    // if it fails we have to clean it better???
    // like pop off stuff and clean the vars inside???

    delete(stack_symbol_table);

    return passed; // Semantic analysis passed without errors

}

int traverse(vector<vector<char*>*>* stack_symbol_table, astNode* node, bool& passed){
    
    node_type type = node->type;

    // BLOCK TYPE and FUNCTION BODY
    if (type == ast_stmt && node->stmt.type == ast_block && body_of_func){    
        vector<char*>* curr_sym_table = stack_symbol_table->back();

        body_of_func = false;
        for (int i = 0; i < node->stmt.block.stmt_list->size(); i++){
            traverse(stack_symbol_table, node->stmt.block.stmt_list->at(i), passed);
        }
    }
    
    // BLOCK TYPE
    else if (type == ast_stmt && node->stmt.type == ast_block){
        vector<char*>* curr_sym_table = new vector<char*> ();
        stack_symbol_table->push_back(curr_sym_table);
        
        for (int i = 0; i < node->stmt.block.stmt_list->size(); i++){
            traverse(stack_symbol_table, node->stmt.block.stmt_list->at(i), passed);
        }

        stack_symbol_table->pop_back();
        delete curr_sym_table;

    } 
    
    // FUNCTION TYPE
    else if (type == ast_func){
        vector<char*>* curr_sym_table = new vector<char*> ();
        stack_symbol_table->push_back(curr_sym_table);

        if (node->func.param != NULL) {
            curr_sym_table->push_back(node->func.param->var.name);
        }

        body_of_func = true;
        traverse(stack_symbol_table, node->func.body, passed);

        stack_symbol_table->pop_back();
        delete curr_sym_table;
    } 
    
    // DECLARATION TYPE
    else if (type == ast_stmt && node->stmt.type == ast_decl){
        char* var_name = node->stmt.decl.name;

        vector<char*>* curr_sym_table = stack_symbol_table->back();

        bool in_table = false;

        for (int j = 0; j < curr_sym_table->size(); j++) {
            if (strcmp(curr_sym_table->at(j), var_name) == 0) {
                in_table = true;   
            } 
        }

        if (in_table){
            passed = false; // did not pass
        } else {
            curr_sym_table->push_back(var_name);
        }
        
    } 
    
    // VARIABLE TYPE
    else if (type == ast_var){
        char* var_name = node->var.name;

        bool found = false;

        for (const vector<char*>* sym_table : *stack_symbol_table){
            for (const char* var : *sym_table){
                if (strcmp(var, var_name) == 0){ // found stack symbol
                    found = true;
                    break;
                }
            }
            if (found) break; // case that we don't need
        }
        
        // variable is not found, print error
        if (!found) {
            passed = false;
        }

    } 
    
    // EXTERN TYPE
    else if (type == ast_extern) {

    }

    // OTHER TYPES
    else if (type == ast_prog) {
        traverse(stack_symbol_table, node->prog.func, passed);
    }

    else if (type == ast_cnst) {

    }

    else if (type == ast_rexpr) {
        // traverse left then right child
        traverse(stack_symbol_table, node->rexpr.lhs, passed);
        traverse(stack_symbol_table, node->rexpr.rhs, passed);

    }

    else if (type == ast_bexpr) {
        traverse(stack_symbol_table, node->bexpr.lhs, passed);
        traverse(stack_symbol_table, node->bexpr.rhs, passed);

    }

    else if (type == ast_uexpr) {
        traverse(stack_symbol_table, node->uexpr.expr, passed);

    }

    else if (type == ast_stmt && node->stmt.type == ast_call){
        if (node->stmt.call.param != NULL) {
            traverse(stack_symbol_table, node->stmt.call.param, passed);
        }

    }

    else if (type == ast_stmt && node->stmt.type == ast_ret){
        traverse(stack_symbol_table, node->stmt.ret.expr, passed);

    }

    else if (type == ast_stmt && node->stmt.type == ast_while){
        // two children
        traverse(stack_symbol_table, node->stmt.whilen.cond, passed);
        traverse(stack_symbol_table, node->stmt.whilen.body, passed);

    }

    else if (type == ast_stmt && node->stmt.type == ast_if){
        // three children
        traverse(stack_symbol_table, node->stmt.ifn.cond, passed);
        traverse(stack_symbol_table, node->stmt.ifn.if_body, passed);

        //check else_body
        if (node->stmt.ifn.else_body != NULL) {
            traverse(stack_symbol_table, node->stmt.ifn.else_body, passed);
        }

    }

    else if (type == ast_stmt && node->stmt.type == ast_asgn){
        // two children
        traverse(stack_symbol_table, node->stmt.asgn.lhs, passed);
        traverse(stack_symbol_table, node->stmt.asgn.rhs, passed);

    }

    if (passed == false) {return false;}


    return true;
}