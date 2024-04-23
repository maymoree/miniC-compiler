#include <vector>
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
void common_sub_expr(LLVMModuleRef module);
void help_print_instructions(LLVMModuleRef module);
void dead_code_elim(LLVMModuleRef module);
void print_vector(vector<LLVMValueRef>* elim_instruction);

vector<LLVMValueRef>* elim_instructions = new vector<LLVMValueRef> ();