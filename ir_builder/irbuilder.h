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

using namespace std;

LLVMModuleRef ir_builder(astNode* root);
void pre_traverse(astNode* node, vector<vector<string>*>* stack_variable_names);
LLVMModuleRef build_traverse(astNode* node, LLVMModuleRef mod);
LLVMBasicBlockRef genIRStmt(astNode* node, LLVMBuilderRef builder, LLVMBasicBlockRef startBB, 
                            map<string, LLVMValueRef>* var_map, LLVMModuleRef mod, LLVMValueRef func);
LLVMValueRef genIRExpr(astNode* node, LLVMBuilderRef builder, map<string, LLVMValueRef>* var_map, LLVMModuleRef mod);