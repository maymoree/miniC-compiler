#include <llvm-c/Core.h>
#include <ast.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>

using namespace std;

void ir_builder(astNode* root);
void pre_traverse(astNode* node, vector<vector<char*>*>* stack_variable_names);

bool bod_of_func;
map<char*, int>* names_tracker_map = new map<char*, int> ();

void ir_builder(astNode* root) {

    vector<vector<char*>*>* stack_variable_names = new vector<vector<char*>*> (); // holds all
    pre_traverse(root, stack_variable_names);

    printNode(root);

    // free and clean up??
    delete(stack_variable_names);
    // map??
}

void pre_traverse(astNode* node, vector<vector<char*>*>* stack_variable_names) {

    
    node_type type = node->type;

    // BLOCK AND FUNCTION BODY --> happens once
    if (type == ast_stmt && node->stmt.type == ast_block && bod_of_func){    
        vector<char*>* curr_sym_table = stack_variable_names->back();

        bod_of_func = false;
        for (int i = 0; i < node->stmt.block.stmt_list->size(); i++){
            pre_traverse(node->stmt.block.stmt_list->at(i), stack_variable_names);
        }
    }

    // BLOCK
    else if (type == ast_stmt && node->stmt.type == ast_block){
        vector<char*>* curr_var_names = new vector<char*> ();
        stack_variable_names->push_back(curr_var_names);
        
        for (int i = 0; i < node->stmt.block.stmt_list->size(); i++){
            pre_traverse(node->stmt.block.stmt_list->at(i), stack_variable_names);
        }

        stack_variable_names->pop_back();
        delete curr_var_names;

    }

    // FUNCTION TYPE
    else if (type == ast_func){
        vector<char*>* curr_var_names = new vector<char*> ();
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

        printf("\n DECLARATION \n");

        char* var_name = node->stmt.decl.name;
        int num_to_add = 0;

        vector<char*>* curr_var_names = stack_variable_names->back();

        // find var_name in names_map
        auto itr = find_if(names_tracker_map->begin(), names_tracker_map->end(), 
                                [var_name](const std::pair<char*, int>& element) { 
                                    return strcmp(element.first, var_name) == 0; 
                                });

        if (itr == names_tracker_map->end()) { // not found
            printf("\nNOT FOUND\n");
            // add name to curr_var_names
            curr_var_names->push_back(var_name);
            names_tracker_map->insert(std::pair<char*, int>(var_name, num_to_add)); // put in name with 0

        } else { // found
            printf("\nFOUND\n");
            num_to_add = ++(itr->second); // increase the int tracker in map
            // change inputs to strings
            string num_to_add_str = to_string(num_to_add);
            string var_name_str = var_name;
            string new_var_name_str = var_name_str + num_to_add_str;
            // copy to char* to add into curr_var_names
            char* new_name = new char[new_var_name_str.length() + 1];
            strcpy(new_name, new_var_name_str.c_str());
            node->stmt.decl.name = new_name;
            printf("\nname exists in all scopes! --> new name: %s\n", node->stmt.decl.name);
            // add new name to curr_var_names
            curr_var_names->push_back(new_name);
        }

        for(auto it = names_tracker_map->begin(); it != names_tracker_map->end(); ++it)
        {
            printf("Map key: %s value: %d\n", it->first, it->second);
        }
    }


    // VARIABLE TYPE
    else if (type == ast_var) {
        char* var_name = node->var.name;
        vector<char*>* curr_var_names = stack_variable_names->back();

        for (int i = 0; i < curr_var_names->size(); i++) {
            if (strncmp(curr_var_names->at(i), var_name, strlen(var_name)) == 0) {
                node->var.name = curr_var_names->at(i);
                printf("\nfound name in scope! --> changed name: %s\n", node->var.name);
            }
        }
    }

    // EXTERN TYPE
    else if (type == ast_extern) {

    }

    // OTHER TYPES
    else if (type == ast_prog) {
        pre_traverse(node->prog.func, stack_variable_names);
    }

    else if (type == ast_cnst) {

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

    else if (type == ast_stmt && node->stmt.type == ast_call){
        if (node->stmt.call.param != NULL) {
            pre_traverse(node->stmt.call.param, stack_variable_names);
        }

    }

    else if (type == ast_stmt && node->stmt.type == ast_ret){
        pre_traverse(node->stmt.ret.expr, stack_variable_names);
    }

    else if (type == ast_stmt && node->stmt.type == ast_while){
        // two children
        pre_traverse(node->stmt.whilen.cond, stack_variable_names);
        pre_traverse(node->stmt.whilen.body, stack_variable_names);
    }

    else if (type == ast_stmt && node->stmt.type == ast_if){
        // three children
        pre_traverse(node->stmt.ifn.cond, stack_variable_names);
        pre_traverse(node->stmt.ifn.if_body, stack_variable_names);

        //check else_body
        if (node->stmt.ifn.else_body != NULL) {
            pre_traverse(node->stmt.ifn.else_body, stack_variable_names);
        }

    }

    else if (type == ast_stmt && node->stmt.type == ast_asgn){
        // two children
        pre_traverse(node->stmt.asgn.lhs, stack_variable_names);
        pre_traverse(node->stmt.asgn.rhs, stack_variable_names);
    }

}