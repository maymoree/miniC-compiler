#include <llvm-c/Core.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <map>
#include <vector>
#include <set>

using namespace std;

map<LLVMValueRef, int> register_allocation(LLVMModuleRef mod);
void compute_liveness(LLVMBasicBlockRef basicBlock, map<LLVMValueRef, int> &inst_index, map<LLVMValueRef, pair<int, int>> &live_range);
// LLVMValueRef find_spill(LLVMValueRef instruc, map<LLVMValueRef, int> &reg_map, map<LLVMValueRef, int> &inst_index,
//                         vector<LLVMValueRef> &sorted_list, map<LLVMValueRef, pair<int, int>> &live_range);
// map<LLVMBasicBlockRef, char*> createBBLabels(LLVMValueRef func);
// void printDirectives(LLVMModuleRef module, LLVMValueRef function, FILE* fp); // not sure yet
// void printFunctionEnd(FILE* fp); // not sure...
// map<LLVMValueRef, int> getOffsetMap(LLVMValueRef func, int* local_mem); // ???
void generator(LLVMModuleRef mod); // ???
// vector<LLVMValueRef> sortList(vector<LLVMValueRef> &sorted_list); // have to sort list somehow
