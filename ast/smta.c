#include "ast.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

int semantic_analysis(astNode* root);
void traverse(vector<vector<char*>*> *stack_symbol_table, astNode* node);

int semantic_analysis(astNode* root){
    vector<vector<char*>*>* stack_symbol_table = new vector<vector<char*>*> (); // holds all

    if (root != NULL) {
        traverse(stack_symbol_table, root);
    }

    delete(stack_symbol_table);

    return (0);

}

void traverse(vector<vector<char*>*>* stack_symbol_table, astNode* node){
    
    node_type type = node->type;

    // BLOCK TYPE and FUNCTION BODY
    // also checks that stack isn't empty, meaning there is prior code block
    // also checks that top symbol stack isn't empty as well, meaning there is func name
    if (type == ast_stmt && node->stmt.type == ast_block && stack_symbol_table->size() > 0 && stack_symbol_table->back()->size() > 0){
        vector<char*>* curr_sym_table = stack_symbol_table->back();

        for (int i = 0; i < node->stmt.block.stmt_list->size(); i++){
            traverse(stack_symbol_table, node->stmt.block.stmt_list->at(i));
        }

        delete curr_sym_table;
    }
    
    // BLOCK TYPE
    else if (type == ast_stmt && node->stmt.type == ast_block){

        vector<char*>* curr_sym_table = new vector<char*> ();
        stack_symbol_table->push_back(curr_sym_table);
        
        for (int i = 0; i < node->stmt.block.stmt_list->size(); i++){
            traverse(stack_symbol_table, node->stmt.block.stmt_list->at(i));
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

        traverse(stack_symbol_table, node->func.body);

        stack_symbol_table->pop_back();
        delete curr_sym_table;
    } 
    
    // DECLARATION TYPE
    else if (type == ast_stmt && node->stmt.type == ast_decl){
        const char* var_name = node->stmt.decl.name;

        vector<char*>* curr_sym_table = stack_symbol_table->back();

        for (const char* var : *curr_sym_table) {
            // variable exists in top symbol table
            if (strcmp(var, var_name) == 0) {
                fprintf(stderr, "Error: Variable already declared in this code block.\n");
                exit(1);
            } else {
                curr_sym_table->push_back(strdup(var_name));
            }
        }

        delete curr_sym_table;
    } 
    
    // VARIABLE TYPE
    else if (type == ast_var){
        const char* var_name = node->var.name;

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
            fprintf(stderr, "Error: Variable '%s' is used but not declared.\n", var_name);
            exit(1);
        }
    } 
    
    // EXTERN TYPE
    else if (type == ast_extern) {
        
    }

    // OTHER TYPES
    else if (type == ast_cnst) {

    }

    else if (type == ast_rexpr) {
        // traverse left then right child
        traverse(stack_symbol_table, node->rexpr.lhs);
        traverse(stack_symbol_table, node->rexpr.rhs);
    }

    else if (type == ast_bexpr) {
        traverse(stack_symbol_table, node->bexpr.lhs);
        traverse(stack_symbol_table, node->bexpr.rhs);
    }

    else if (type == ast_uexpr) {
        traverse(stack_symbol_table, node->uexpr.expr);
    }

    else if (type == ast_stmt && node->stmt.type == ast_call){
        if (node->stmt.call.param != NULL) {
            traverse(stack_symbol_table, node->stmt.call.param);
        }
    }

    else if (type == ast_stmt && node->stmt.type == ast_ret){
        traverse(stack_symbol_table, node->stmt.ret.expr);
    }

    else if (type == ast_stmt && node->stmt.type == ast_while){
        // two children
        traverse(stack_symbol_table, node->stmt.whilen.cond);
        traverse(stack_symbol_table, node->stmt.whilen.body);
    }

    else if (type == ast_stmt && node->stmt.type == ast_if){
        // three children
        traverse(stack_symbol_table, node->stmt.ifn.cond);
        traverse(stack_symbol_table, node->stmt.ifn.if_body);

        //check else_body
        if (node->stmt.ifn.else_body != NULL) {
            traverse(stack_symbol_table, node->stmt.ifn.else_body);
        }
    }

    else if (type == ast_stmt && node->stmt.type == ast_asgn){
        // two children
        traverse(stack_symbol_table, node->stmt.asgn.lhs);
        traverse(stack_symbol_table, node->stmt.asgn.rhs);
    }

    else {}

}