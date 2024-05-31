#include <llvm-c/Core.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <algorithm>


using namespace std;

map<LLVMValueRef, int> register_allocation(LLVMModuleRef mod, map<LLVMValueRef, int> &inst_index, map<LLVMValueRef, pair<int, int>> &live_range);
void compute_liveness(LLVMBasicBlockRef basicBlock, map<LLVMValueRef, int> &inst_index, map<LLVMValueRef, pair<int, int>> &live_range);
LLVMValueRef find_spill(LLVMValueRef instruc, map<LLVMValueRef, int> &reg_map, map<LLVMValueRef, int> &inst_index,
                        vector<LLVMValueRef> &sorted_list, map<LLVMValueRef, pair<int, int>> &live_range);
map<LLVMBasicBlockRef, string> createBBLabels(LLVMValueRef func);
void printDirectives(LLVMModuleRef module, LLVMValueRef function, FILE* fp);
void printFunctionEnd(FILE* fp);
map<LLVMValueRef, int> getOffsetMap(LLVMModuleRef mod, int* local_mem);
void generator(LLVMModuleRef mod);
vector<LLVMValueRef> sort_list(map<LLVMValueRef, pair<int, int>> &live_range);
bool isInstructionWithoutResult(LLVMValueRef instruc);
const char* get_reg_name(int reg);
const char* get_operator(LLVMOpcode op);