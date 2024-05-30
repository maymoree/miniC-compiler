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
// map<LLVMBasicBlockRef, char*> createBBLabels(LLVMValueRef func);
// void printDirectives(LLVMModuleRef module, LLVMValueRef function, FILE* fp); // not sure yet
// void printFunctionEnd(FILE* fp); // not sure...
// map<LLVMValueRef, int> getOffsetMap(LLVMValueRef func, int* local_mem); // ???
void generator(LLVMModuleRef mod); // ???
vector<LLVMValueRef> sort_list(map<LLVMValueRef, pair<int, int>> &live_range);

map<int, string> REGISTER_MAP = {
    {0, "ebx"},
    {1, "ecx"},
    {2, "edx"},
    {-1, "spill"}
};

map<LLVMValueRef, int> register_allocation(LLVMModuleRef mod, map<LLVMValueRef, int> &inst_index, map<LLVMValueRef, pair<int, int>> &live_range) {
    // only for those instructions that generate value (spilled = -1)

    map<LLVMValueRef, int> reg_map;
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

            set<int> registers {0, 1, 2}; // initialize available registers

            compute_liveness(basicBlock, inst_index, live_range);

            for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock); 
            instruction; 
            instruction = LLVMGetNextInstruction(instruction)) {

                LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);

                // alloc instructions
                if (instruc_opcode == LLVMAlloca) { // check if it's an alloc instruction
                    continue;
                }

                if (instruc_opcode == LLVMStore || instruc_opcode == LLVMBr || instruc_opcode == LLVMCall) { // no result instructions
                    
                    // loop through instruc's operands to check if any of it has ended
                    int num_operands = LLVMGetNumOperands(instruction);
                    for (int i = 0; i < num_operands; i++) {
                        LLVMValueRef op = LLVMGetOperand(instruction, i);
                        if (live_range.count(op) && reg_map.count(op)) { // if operand exists in live_range and reg_map
                            // if the instruction ends, and it is assigned to a register (not spill)
                            if (live_range.at(op).second == inst_index.at(instruction) && reg_map.at(op) != -1) {
                                registers.insert(reg_map.at(op));
                            }
                        }
                    }

                }
                else { 
                    if (instruc_opcode == LLVMAdd || instruc_opcode == LLVMSub || instruc_opcode == LLVMMul) { // if add sub or mul

                        LLVMValueRef first_op = LLVMGetOperand(instruction, 0);

                        // check if first op has physical register and it's liveness range ends at instruction
                        if (reg_map.count(first_op) && reg_map.at(first_op) != -1 && live_range.at(first_op).second == inst_index.at(instruction)) {
                            
                            // add free register to reg_map with instruction
                            int reg = reg_map.at(first_op);
                            registers.erase(reg); // might be redundant
                            reg_map.insert(pair<LLVMValueRef, int> (instruction, reg));

                            // add free register from second operand to available registers
                            LLVMValueRef second_op = LLVMGetOperand(instruction, 1);
                            if (reg_map.count(second_op) && reg_map.at(second_op) != -1 && live_range.at(second_op).second == inst_index.at(instruction)) {
                                registers.insert(reg_map.at(second_op));
                            }

                        }

                    }
                    else if (!registers.empty()) { // if there are registers available

                        // get a register and add it to reg_map with current instruction
                        auto first_reg = registers.begin(); 
                        int reg = *first_reg;
                        reg_map.insert(pair<LLVMValueRef, int> (instruction, reg));
                        registers.erase(reg); 

                        // loop through instruc's operands to check if any of it has ended
                        int num_operands = LLVMGetNumOperands(instruction);
                        for (int i = 0; i < num_operands; i++) {
                            LLVMValueRef op = LLVMGetOperand(instruction, i);
                            if (live_range.count(op) && reg_map.count(op)) { // if operand exists in live_range and reg_map
                                // if the instruction ends, and it is assigned to a register (not spill)
                                if (live_range.at(op).second == inst_index.at(instruction) && reg_map.at(op) != -1) {
                                    registers.insert(reg_map.at(op));
                                }
                            }
                        }

                    }
                    else {
                        vector<LLVMValueRef> sorted_list = sort_list(live_range);
                        LLVMValueRef spill = find_spill(instruction, reg_map, inst_index, sorted_list, live_range);

                        // use liveness range to compare; if liveness range of instruc ends after that of V
                        if (live_range.at(spill).second < live_range.at(instruction).second) {
                            reg_map.insert(pair<LLVMValueRef, int> (instruction, -1));
                        } else {
                            int reg = reg_map.at(spill);
                            reg_map.insert(pair<LLVMValueRef, int> (instruction, reg));
                            
                            reg_map.erase(spill);
                            reg_map.insert(pair<LLVMValueRef, int> (spill, -1));
                        }

                        // loop through instruc's operands to check if any of it has ended
                        int num_operands = LLVMGetNumOperands(instruction);
                        for (int i = 0; i < num_operands; i++) {
                            LLVMValueRef op = LLVMGetOperand(instruction, i);
                            if (live_range.count(op) && reg_map.count(op)) { // if operand exists in live_range and reg_map
                                // if the instruction ends, and it is assigned to a register (not spill)
                                if (live_range.at(op).second == inst_index.at(instruction) && reg_map.at(op) != -1) {
                                    registers.insert(reg_map.at(op));
                                }
                            }
                        }
                            
                    }
                
                }
                
            }
        }

    }

    return reg_map;
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
    // printf("\ninst index\n");
    // for (auto& entry : inst_index) {
    //     LLVMValueRef instr = entry.first;
    //     int index = entry.second;
    //     LLVMDumpValue(instr);
    //     printf(" --> index: %d\n", index);
    // }
    // printf("\nlive ranges\n");
    // for (const auto& entry : live_range) {
    //     LLVMValueRef instr = entry.first;
    //     pair<int, int> range = entry.second;
    //     LLVMDumpValue(instr);
    //     printf(" --> range: %d - %d \n", range.first, range.second);
    // }

}

// sort list by the endpoints in live_map (decreasing order)
vector<LLVMValueRef> sort_list(map<LLVMValueRef, pair<int, int>> &live_range) {
    vector<LLVMValueRef> sorted;

    // Copy keys from live_range to sorted vector
    for (const auto &instruc : live_range) {
        sorted.push_back(instruc.first);
    }

    // Custom comparator to sort by the endpoint in live_range in decreasing order
    sort(sorted.begin(), sorted.end(), [&live_range](LLVMValueRef a, LLVMValueRef b) {
        return live_range[a].second > live_range[b].second;
    });

    return sorted;
}

LLVMValueRef find_spill(LLVMValueRef instruc, map<LLVMValueRef, int> &reg_map, map<LLVMValueRef, int> &inst_index, vector<LLVMValueRef> &sorted_list, map<LLVMValueRef, pair<int, int>> &live_range) {

    pair<int, int> instruc_live_range = live_range.at(instruc);

    for (LLVMValueRef V : sorted_list) {

        pair<int, int> V_live_range = live_range.at(V);

        // check if V_live_range overlaps with instruc_live_range
        // 1. V ends after instruc starts
        // 2. V starts before instruc ends
        bool overlap = (V_live_range.second > instruc_live_range.first || V_live_range.first < instruc_live_range.second);
        if (overlap) {
            // check if V is in reg_map and not -1 (not spilled)
            if (reg_map.count(V) && reg_map.at(V) != -1) {
                return V;
            }
        }
    }

    return NULL;
}

void generator(LLVMModuleRef mod) {

    // populates the maps that already exist --> have to pass them in as parameters
    map<LLVMValueRef, int> inst_index;
    map<LLVMValueRef, pair<int, int>> live_range;

    map<LLVMValueRef, int> map_1 = register_allocation(mod, inst_index, live_range);

    printf("\nreg_map!!!\n");
    for (const auto& itr : map_1) {
        LLVMValueRef instr = itr.first;
        int reg = itr.second;
        LLVMDumpValue(instr);
        printf(" --> reg: %d \n", reg);
    }

}