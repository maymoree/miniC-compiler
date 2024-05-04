#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm> 
#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>
#include <llvm-c/Types.h>

using namespace std;

#define prt(x) if(x) { printf("%s\n", x); }

LLVMModuleRef createLLVMModel(char * filename);
vector<LLVMValueRef>* common_sub_expr(LLVMModuleRef module);
void dead_code_elim(LLVMModuleRef module, vector<LLVMValueRef>* elim_instructions);
vector<LLVMValueRef>* const_folding (LLVMModuleRef module);
void help_print_instructions(LLVMModuleRef module);
void print_vector(vector<LLVMValueRef>* elim_instruction);
void compute_gen(LLVMModuleRef module, unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* gen_map);
set<LLVMValueRef>* find_all_stores(LLVMModuleRef module); 
void compute_in_out (LLVMModuleRef module, 
					unordered_map<LLVMBasicBlockRef, std::set<LLVMValueRef>*>* in_map, 
					unordered_map<LLVMBasicBlockRef, std::set<LLVMValueRef>*>* out_map, 
					unordered_map<LLVMBasicBlockRef, std::set<LLVMValueRef>*>* gen_map, 
					unordered_map<LLVMBasicBlockRef, std::set<LLVMValueRef>*>* kill_map);
bool delete_load(LLVMModuleRef module,
				unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* in_map,
				unordered_map<LLVMBasicBlockRef,set<LLVMValueRef>*>* kill_map);
bool local_constant_folding(LLVMModuleRef module);
bool global_constant_propagation(LLVMModuleRef module);
void optimize(LLVMModuleRef module);
void main_optimization(LLVMModuleRef m);