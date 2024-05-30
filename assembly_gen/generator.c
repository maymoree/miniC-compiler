#include <llvm-c/Core.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <iostream>

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

map<int, string> REGISTER_MAP = {
    {0, "ebx"},
    {1, "ecx"},
    {2, "edx"},
    {-1, "spill"}
};

map<LLVMValueRef, int> register_allocation(LLVMModuleRef mod) {
    // only for those instructions that generate value (spilled = -1)

    map<LLVMValueRef, int> map_return;
    // loop through functions
	for (LLVMValueRef function = LLVMGetFirstFunction(mod); 
	function; 
	function = LLVMGetNextFunction(function)) {

        printf("\nin func\n");

		// loop through basic block
		for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
            basicBlock;
            basicBlock = LLVMGetNextBasicBlock(basicBlock)) {

            printf("\nin block\n");

            set<int> registers {0, 1, 2}; // initialize registers

            // populates the maps that already exist
            map<LLVMValueRef, int> inst_index;
            map<LLVMValueRef, pair<int, int>> live_range;

            compute_liveness(basicBlock, inst_index, live_range);


            // for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock); 
            // instruction; 
            // instruction = LLVMGetNextInstruction(instruction)) {

            // }
        }

    }

    return map_return;
}

void compute_liveness(LLVMBasicBlockRef basicBlock, map<LLVMValueRef, int> &inst_index, map<LLVMValueRef, pair<int, int>> &live_range) {
    // local liveness

    printf("\ncompute!!!\n");


    // first loop assigning indices; ignoring allocs
    int idx = 0;
    for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock); 
        instruction; 
        instruction = LLVMGetNextInstruction(instruction)) {
		
        LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);
        if (instruc_opcode == LLVMAlloca) { // check if it's an alloc instruction
            continue;
        }

        inst_index.insert(pair<LLVMValueRef, int>(instruction, idx));

        idx += 1;
    }

    // second loop to determine liveness; ignoring allocs
    for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock); 
        instruction; 
        instruction = LLVMGetNextInstruction(instruction)) {

        LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);
        if (instruc_opcode == LLVMAlloca) { // check if it's an alloc instruction
            continue;
        }

        // gets the first use of the instruction
        LLVMUseRef first_use = LLVMGetFirstUse(instruction);

        if (first_use == NULL) { // there is no use of the instruction
            int use_idx = inst_index.at(instruction);
            pair<LLVMValueRef, pair<int, int>> use_pair(instruction, pair<int, int> (use_idx, use_idx));
            live_range.insert(use_pair);
        }
        else { // there is use of the instruction
            LLVMValueRef last_use_instruc = LLVMGetUser(first_use); // set last use instruc to the instruc of first use

            // loops to next use until it becomes NULL
            for (LLVMUseRef use = first_use; use != NULL; use = LLVMGetNextUse(use)) {
                last_use_instruc = LLVMGetUser(use); // grabs instruction that uses it and update 
            }

            pair<int, int> startEnd (inst_index.at(instruction), inst_index.at(last_use_instruc));
            pair<LLVMValueRef, pair<int, int>> liveness (instruction, startEnd);
            live_range.insert(liveness);
        }
    }

    // check if liveness is correct
    // Print inst_index for debugging
    printf("\ninst index\n");
    for (auto& entry : inst_index) {
        LLVMValueRef instr = entry.first;
        int index = entry.second;
        LLVMDumpValue(instr);
        printf(" --> index: %d\n", index);
    }

    // Print live_range for debugging
    printf("\nlive ranges\n");
    for (const auto& entry : live_range) {
        LLVMValueRef instr = entry.first;
        pair<int, int> range = entry.second;
        LLVMDumpValue(instr);
        printf(" --> range: %d - %d \n", range.first, range.second);
    }

}

void generator(LLVMModuleRef mod) {

    map<LLVMValueRef, int> map_1 = register_allocation(mod);

}