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

typedef enum {
    EAX,
    EBX,
    ECX,
    EDX,
} reg_t;

map<LLVMValueRef, int> register_allocation(LLVMModuleRef mod, map<LLVMValueRef, int> &inst_index, map<LLVMValueRef, pair<int, int>> &live_range) {
    // only for those instructions that generate value (spilled = -1)

    map<LLVMValueRef, int> reg_map;
    // loop through functions
	for (LLVMValueRef function = LLVMGetFirstFunction(mod); 
	function; 
	function = LLVMGetNextFunction(function)) {

		// loop through basic block
		for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
            basicBlock;
            basicBlock = LLVMGetNextBasicBlock(basicBlock)) {

            set<int> registers {EBX, ECX, EDX}; // initialize available registers

            compute_liveness(basicBlock, inst_index, live_range);

            for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock); 
            instruction; 
            instruction = LLVMGetNextInstruction(instruction)) {

                LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);

                // alloc instructions
                if (instruc_opcode == LLVMAlloca) { // check if it's an alloc instruction
                    continue;
                }

                if (isInstructionWithoutResult(instruction)) { // no result instructions

                    // loop through instruc's operands to check if any of it has ended
                    int num_operands = LLVMGetNumOperands(instruction);
                    for (int i = 0; i < num_operands; i++) {
                        LLVMValueRef op = LLVMGetOperand(instruction, i);
                        if (live_range.count(op) && reg_map.count(op)) { // if operand exists in live_range and reg_map
                            // if the instruction ends, and it is assigned to a register (not -1)
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
                            reg_map.insert(pair<LLVMValueRef, int> (instruction, reg));

                            // add free register from second operand to available registers
                            LLVMValueRef second_op = LLVMGetOperand(instruction, 1);
                            if (reg_map.count(second_op) && reg_map.at(second_op) != -1 && live_range.at(second_op).second == inst_index.at(instruction)) {
                                registers.insert(reg_map.at(second_op));
                            }
                            continue; // if able to insert to reg_map, then move to next instruction
                        }

                    }
                    if (!registers.empty()) { // if there are registers available

                        // get a register and add it to reg_map with current instruction
                        auto first_reg = registers.begin(); 
                        int reg = *first_reg;
                        registers.erase(reg); 
                        reg_map.insert(pair<LLVMValueRef, int> (instruction, reg));

                        // loop through instruc's operands to check if any of it has ended
                        int num_operands = LLVMGetNumOperands(instruction);
                        for (int i = 0; i < num_operands; i++) {
                            LLVMValueRef op = LLVMGetOperand(instruction, i);
                            if (live_range.count(op) && reg_map.count(op)) { // if operand exists in live_range and reg_map
                                // if the instruction ends, and it is assigned to a register (not -1)
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
                        } else { // choose the one that ends first!
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
                                // if the instruction ends, and it is assigned to a register (not -1)
                                if (live_range.at(op).second == inst_index.at(instruction) && reg_map.at(op) != -1) {
                                    registers.insert(reg_map.at(op));
                                }
                            }
                        }
                            
                    }
                
                }
                
            }

            inst_index.clear();
            live_range.clear();

        }

    }

    return reg_map;
}

void compute_liveness(LLVMBasicBlockRef basicBlock, map<LLVMValueRef, int> &inst_index, map<LLVMValueRef, pair<int, int>> &live_range) {
    // local liveness

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

        LLVMUseRef first_use = LLVMGetFirstUse(instruction);
        
        if (first_use == NULL) { // there is no use of the instruction
            int use_idx = inst_index.at(instruction);
            pair<LLVMValueRef, pair<int, int>> use_pair(instruction, pair<int, int> (use_idx, use_idx));
            live_range.insert(use_pair);
        }
        else { // it if was used at least once
            LLVMValueRef last_use_instruc = NULL;

            for (LLVMValueRef use_inst = LLVMGetUser(first_use); use_inst; use_inst = LLVMGetNextInstruction(use_inst)) {
                int num_operands = LLVMGetNumOperands(use_inst);
                for (int i = 0; i < num_operands; i++) {
                    if (instruction == LLVMGetOperand(use_inst, i)) {
                        last_use_instruc = use_inst;
                    }
                }
            }
            pair<int, int> range (inst_index.at(instruction), inst_index.at(last_use_instruc));
            pair<LLVMValueRef, pair<int, int>> use_pair (instruction, range);
            live_range.insert(use_pair);
        }
    }
}


// GENERATOR ---------------------------------------------------------------------
void generator(LLVMModuleRef mod) {

    FILE* fp = fopen("pt4.s", "w");

    //---------------------------------------------------------------------
    // Register Allocation
    //---------------------------------------------------------------------

    // populates the maps that already exist --> have to pass them in as parameters
    map<LLVMValueRef, int> inst_index;
    map<LLVMValueRef, pair<int, int>> live_range;

    map<LLVMValueRef, int> reg_map = register_allocation(mod, inst_index, live_range);

    printf("\n Reg_Map \n");
    for (auto &instruc : reg_map) {
        LLVMDumpValue(instruc.first);
        const char* reg = get_reg_name(instruc.second);
        printf(" --> register: %d, %s\n", instruc.second, reg);
    }

    //---------------------------------------------------------------------
    // Code Generation
    //---------------------------------------------------------------------

    for (LLVMValueRef function = LLVMGetLastFunction(mod); 
	function; 
	function = LLVMGetNextFunction(function)) {

        map<LLVMBasicBlockRef, string> bb_labels = createBBLabels(function);

        printDirectives(mod, function, fp);

        int local_mem;
        map<LLVMValueRef, int> offset_map = getOffsetMap(mod, &local_mem);

        fprintf(fp, "\tpushl\t%%ebp\n");
        fprintf(fp, "\tmovl\t%%esp, %%ebp\n");
        fprintf(fp, "\tsubl\t$%d, %%esp\n", local_mem);
        fprintf(fp, "\tpushl\t%%ebx\n");

        for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
        basicBlock;
        basicBlock = LLVMGetNextBasicBlock(basicBlock)) {
            
            // print bb label
            fprintf(fp, "\n%s:\n", bb_labels.at(basicBlock).c_str());

            for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock); 
            instruction; 
            instruction = LLVMGetNextInstruction(instruction)){

                LLVMOpcode op = LLVMGetInstructionOpcode(instruction);

                if (op == LLVMRet) {
                    LLVMValueRef operand = LLVMGetOperand(instruction, 0);
                    if (LLVMIsConstant(operand)) { // constant
                        // emit movl $A, %eax
                        int const_A = LLVMConstIntGetSExtValue(operand);
                        fprintf(fp, "\tmovl\t$%d, %%eax\n", const_A);
                    }
                    else if (reg_map.count(operand) && reg_map.at(operand) == -1) { // stored in memory
                        // get offset k of A
                        int offset_A = offset_map.at(operand);
                        // emit movl k(%ebp), %eax 
                        fprintf(fp, "\tmovl\t%d(%%ebp), %%eax\n", offset_A);
                    }
                    else if (reg_map.count(operand) && reg_map.at(operand) != -1) { // stored in reg
                        const char* reg_name = get_reg_name(reg_map.at(operand));
                        // emit movl %exx, %eax
                        fprintf(fp, "\tmovl\t%%%s, %%eax\n", reg_name);
                    }
                    // emit popl %ebx
                    fprintf(fp, "\tpopl\t %%ebx\n");
                    // call printFunctionEnd  (leave, ret)
                    printFunctionEnd(fp);
                }
                else if (op == LLVMLoad) {
                    // instruction is a, operand is b
                    LLVMValueRef operand = LLVMGetOperand(instruction, 0);
                    if (reg_map.count(instruction) && reg_map.at(instruction) != -1) {
                        int offset_C = offset_map.at(operand);
                        const char* reg_name = get_reg_name(reg_map.at(instruction));
                        // emit movl c(%ebp),%exx
                        fprintf(fp, "\tmovl\t %d(%%ebp),%%%s\n", offset_C, reg_name);
                    }
                }
                else if (op == LLVMStore) {
                    LLVMValueRef operand_A = LLVMGetOperand(instruction, 0);
                    LLVMValueRef operand_b = LLVMGetOperand(instruction, 1);
                    if (operand_A == LLVMGetParam(function, 0)) {
                    }
                    else if (LLVMIsConstant(operand_A)) {
                        int offset_C = offset_map.at(operand_b);
                        int const_A = LLVMConstIntGetSExtValue(operand_A);
                        // emit movl $A, c(%ebp) 
                        fprintf(fp, "\tmovl\t$%d, %d(%%ebp)\n", const_A, offset_C);
                    } else if (reg_map.count(operand_A) && reg_map.at(operand_A) != -1) { // temp variable and register assigned
                        const char* reg = get_reg_name(reg_map.at(operand_A));
                        int offset_C = offset_map.at(operand_b);
                        // emit movl %exx, c(%ebp)
                        fprintf(fp, "\tmovl\t%%%s, %d(%%ebp)\n", reg, offset_C);
                    }
                    else {
                        int offset_C1 = offset_map.at(operand_A);
                        // emit movl c1(%ebp), %eax
                        fprintf(fp, "\tmovl\t%d(%%ebp), %%eax\n", offset_C1);
                        int offset_C2 = offset_map.at(operand_b);
                        // emit movl %eax, c2(%ebp)
                        fprintf(fp, "\tmovl\t%%eax, %d(%%ebp)\n", offset_C2);
                    }
                }
                else if (op == LLVMCall) {

                    LLVMValueRef func_a = LLVMGetCalledValue(instruction);

                    //emit pushl %ecx
                    //emit pushl %edx
                    fprintf(fp, "\tpushl\t%%ecx\n");
                    fprintf(fp, "\tpushl\t%%edx\n");

                    if (LLVMCountParams(func_a)) { // has param p
                        LLVMValueRef param_p = LLVMGetParam(func_a, 0);
                        if (LLVMIsConstant(param_p)) {
                            int const_param_p = LLVMConstIntGetSExtValue(param_p);
                            // emit pushl $P
                            fprintf(fp, "\tpushl\t$%d\n", const_param_p);
                        }
                        else if (reg_map.count(param_p)) {
                            if (reg_map.at(param_p) != -1) { // has register
                                const char* reg = get_reg_name(reg_map.at(param_p));
                                // emit pushl %exx
                                fprintf(fp, "\tpushl\t$%%%s\n", reg);
                            } else { // in memory; no register
                                int offset_K = offset_map.at(param_p);
                                // emit pushl k(%ebp)
                                fprintf(fp, "\tpushl\t%d(%%ebp)\n", offset_K);
                            }
                        }
                    }
                    // emit call func
                    size_t call_len;
                    const char* call_name = LLVMGetValueName2(func_a, &call_len);
                    fprintf(fp, "\tcall\t%s\n", call_name);

                    if (LLVMCountParams(func_a)) {
                        // emit addl $4, %esp (this is to undo the pushing of the parameter)
                        fprintf(fp, "\taddl\t$4, %%esp\n");
                    }

                    // emit popl %edx
                    // emit popl %ecx
                    fprintf(fp, "\tpopl\t%%edx\n");
                    fprintf(fp, "\tpopl\t%%ecx\n");
 
                    // has no parameter --> read() + has result (LHS)
                    if (!LLVMCountParams(func_a) && !isInstructionWithoutResult(instruction)) {
                        if (reg_map.at(instruction) != -1) { // has register
                            const char* reg = get_reg_name(reg_map.at(instruction));
                            // emit movl %eax, %exx
                            fprintf(fp, "\tmovl\t%%eax, %%%s\n", reg);
                        } else { // no register; in memory
                            int offset_K = offset_map.at(instruction);
                            // emit movl %eax, k(%ebp)
                            fprintf(fp, "\tmovl\t%%eax, %d(%%ebp)\n", offset_K);
                        }
                    }
                }
                else if (op == LLVMBr) {
                    if (!LLVMIsConditional(instruction)) { // unconditional
                        // get label L of %b from bb_labels
                        // need basicBlockRef -> LLVMBasicBlockRef LLVMValueAsBasicBlock(LLVMValueRef Val);
                        LLVMValueRef operand_b = LLVMGetOperand(instruction, 0);
                        LLVMBasicBlockRef bb_b = LLVMValueAsBasicBlock(operand_b);
                        string label_L = bb_labels.at(bb_b);
                        // emit jmp L
                        fprintf(fp, "\tjmp\t%s\n", label_L.c_str());
                    } else {
                        LLVMValueRef operand_b = LLVMGetOperand(instruction, 2);
                        LLVMValueRef operand_c = LLVMGetOperand(instruction, 1);

                        LLVMBasicBlockRef bb_b = LLVMValueAsBasicBlock(operand_b);
                        LLVMBasicBlockRef bb_c = LLVMValueAsBasicBlock(operand_c);

                        string label_L1 = bb_labels.at(bb_b);
                        string label_L2 = bb_labels.at(bb_c);

                        // get predicate T -> LLVMIntPredicate LLVMGetICmpPredicate(LLVMValueRef Inst);
                        LLVMValueRef operand_a = LLVMGetOperand(instruction, 0);
                        LLVMIntPredicate pred_T = LLVMGetICmpPredicate(operand_a);

                        switch (pred_T) { // (<, >, <=, >=, ==)
                            case (LLVMIntSLT): {
                                fprintf(fp, "\tjlt\t%s\n", label_L1.c_str());
                                break;
                            }
                            case LLVMIntSGT: {
                                fprintf(fp, "\tjgt\t%s\n", label_L1.c_str());
                                break;
                            }
                            case LLVMIntSLE: {
                                fprintf(fp, "\tjle\t%s\n", label_L1.c_str());
                                break;
                            }
                            case LLVMIntSGE: {
                                fprintf(fp, "\tjge\t%s\n", label_L1.c_str());
                                break;
                            }
                            case LLVMIntEQ: {
                                fprintf(fp, "\tjeq\t%s\n", label_L1.c_str());
                                break;
                            }
                            default: {
                                break;
                            }
                        }

                        // emit jmp L2 
                        fprintf(fp, "\tjmp\t%s\n", label_L2.c_str());
                    }
                }
                else if (op == LLVMAdd || op == LLVMMul || op == LLVMSub) {

                    int reg_int;
                    if (reg_map.at(instruction) != -1) {
                        reg_int = reg_map.at(instruction);
                    } else {
                        reg_int = EAX;
                    }

                    LLVMValueRef operand_a = LLVMGetOperand(instruction, 0);

                    if (LLVMIsConstant(operand_a)) {
                        int const_a = LLVMConstIntGetSExtValue(operand_a);
                        const char* reg = get_reg_name(reg_int);
                        // emit movl $A, R
                        fprintf(fp, "\tmovl\t$%d, %s\n", const_a, reg);
                    }
                    else if (reg_map.at(operand_a)) {
                        if (reg_map.at(operand_a) != -1) { // has register
                            // emit movl %eyy, R  (do not emit if both registers are the same)
                            if (reg_map.at(operand_a) != reg_int) {
                                const char* reg = get_reg_name(reg_int);
                                const char* reg_a = get_reg_name(reg_map.at(operand_a));
                                fprintf(fp, "\tmovl\t%%%s, %s\n", reg_a, reg);
                            }
                        } else {
                            int offset_n = offset_map.at(operand_a);
                            const char* reg = get_reg_name(reg_int);
                            // emit movl n(%ebp), R
                            fprintf(fp, "\tmovl\t%d(%%ebp), %s\n", offset_n, reg);
                        }
                    }

                    LLVMValueRef operand_b = LLVMGetOperand(instruction, 1);

                    if (LLVMIsAConstantInt(operand_b)) {
                        int const_b = LLVMConstIntGetSExtValue(operand_b);
                        const char* reg = get_reg_name(reg_int);
                        const char* opt = get_operator(op); // grab operator string according to opcode
                        // emit addl/imull/subl $B, R
                        fprintf(fp, "\t%s\t$%d, %%%s\n", opt, const_b, reg);
                    }
                    else if (reg_map.count(operand_b) && reg_map.at(operand_b) != -1) {
                        const char* reg = get_reg_name(reg_int);
                        const char* reg_b = get_reg_name(reg_map.at(operand_b));
                        const char* opt = get_operator(op);
                        // emit addl/imull/subl %ezz, R
                        fprintf(fp, "\t%s\t%%%s, %%%s\n", opt, reg_b, reg);
                    }
                    else {
                        int offset_m = offset_map.at(operand_b);
                        const char* opt = get_operator(op);
                        const char* reg = get_reg_name(reg_int);
                        // emit addl m(%ebp), R [Note: you can replace addl by subl or imull based on opcode]
                        fprintf(fp, "\t%s\t%d(%%ebp), %%%s\n", opt, offset_m, reg);
                    }

                    if (reg_map.count(instruction)) {
                        int offset_k = offset_map.at(instruction);
                        // emit movl %eax, k(%ebp)
                        fprintf(fp, "\tmovl\t%%eax, %d(%%ebp)\n", offset_k);
                    }

                }
                else if (op == LLVMICmp) {

                    int reg_int;
                    if (reg_map.at(instruction) != -1) {
                        reg_int = reg_map.at(instruction);
                    }
                    else {
                        reg_int = EAX;
                    }

                    LLVMValueRef operand_a = LLVMGetOperand(instruction, 0);

                    if (LLVMIsConstant(operand_a)) {
                        int const_a = LLVMConstIntGetSExtValue(operand_a);
                        const char* reg = get_reg_name(reg_int);
                        // emit movl $A, R
                        fprintf(fp, "\tmovl\t$%d, %s\n", const_a, reg);
                    }
                    else if (reg_map.at(operand_a)) {
                        if (reg_map.at(operand_a) != -1) { // has register
                            // emit movl %eyy, R  (do not emit if both registers are the same)
                            if (reg_map.at(operand_a) != reg_int) {
                                const char* reg = get_reg_name(reg_int);
                                const char* reg_a = get_reg_name(reg_map.at(operand_a));
                                fprintf(fp, "\tmovl\t%%%s, %s\n", reg_a, reg);
                            }
                        } else { // in memory
                            int offset_n = offset_map.at(operand_a);
                            const char* reg = get_reg_name(reg_int);
                            // emit movl n(%ebp), R
                            fprintf(fp, "\tmovl\t%d(%%ebp), %s\n", offset_n, reg);
                        }
                    }

                    LLVMValueRef operand_b = LLVMGetOperand(instruction, 1);

                    if (LLVMIsAConstantInt(operand_b)) {
                        int const_b = LLVMConstIntGetSExtValue(operand_b);
                        const char* reg = get_reg_name(reg_int);
                        // emit cmpl $B, R 
                        fprintf(fp, "\tcmpl\t$%d, %%%s\n", const_b, reg);
                    }
                    else if (reg_map.count(operand_b) && reg_map.at(operand_b) != -1) {
                        const char* reg = get_reg_name(reg_int);
                        const char* reg_b = get_reg_name(reg_map.at(operand_b));
                        // emit cmpl %ezz, R  
                        fprintf(fp, "\tcmpl\t%%%s, %%%s\n", reg_b, reg);
                    }
                    else {
                        int offset_m = offset_map.at(operand_b);
                        const char* opt = get_operator(op);
                        const char* reg = get_reg_name(reg_int);
                        // emit cmpl m(%ebp), R
                        fprintf(fp, "\tcmpl\t%d(%%ebp), %%%s\n", offset_m, reg);
                    }
                }
            }
        }
    }  
    fclose(fp);
}




// HELPER FUNCTION ---------------------------------------------------------------

bool isInstructionWithoutResult(LLVMValueRef instruc) {

    LLVMOpcode opcode = LLVMGetInstructionOpcode(instruc);

    if (opcode == LLVMStore || opcode == LLVMBr || opcode == LLVMRet) {
        return true;
    }

    if (opcode == LLVMCall) {
        LLVMTypeRef call_type = LLVMGetCalledFunctionType(instruc);
        LLVMTypeRef call_ret_type = LLVMGetReturnType(call_type);
        if (call_ret_type && (LLVMGetTypeKind(call_ret_type) == LLVMVoidTypeKind)) {
            return true;
        }
    }

    return false;
}

void printDirectives(LLVMModuleRef module, LLVMValueRef function, FILE* fp) {
    size_t func_len;
    const char *func_name = LLVMGetValueName2(function, &func_len);

    fprintf(fp, "\t.text\n");
    fprintf(fp, "\t.globl\t%s\n", func_name);
    fprintf(fp, "\t.type\t%s, @function\n", func_name);
    fprintf(fp, "%s:\n", func_name);
}

void printFunctionEnd(FILE* fp) {
    fprintf(fp, "\tleave\n");
    fprintf(fp, "\tret\n");
}

map<LLVMBasicBlockRef, string> createBBLabels(LLVMValueRef func) {

    map<LLVMBasicBlockRef, string> labels;

    int count = 0;
    for (LLVMBasicBlockRef bb = LLVMGetFirstBasicBlock(func); bb; bb = LLVMGetNextBasicBlock(bb)) {
        string label = ".BB" + to_string(count);
        labels.insert(pair<LLVMBasicBlockRef, string> (bb, label));
        count += 1;
    }

    return labels;

}

map<LLVMValueRef, int> getOffsetMap(LLVMModuleRef mod, int* local_mem) {

    map<LLVMValueRef, int> offset_map;
    LLVMValueRef param;

    *local_mem = 4;

    for (LLVMValueRef function = LLVMGetFirstFunction(mod); 
	function; 
	function = LLVMGetNextFunction(function)) {

        if (LLVMCountParams(function)) {
            param = LLVMGetParam(function, 0);
            offset_map.insert(pair<LLVMValueRef, int> (param, 8));
        }

		for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
            basicBlock;
            basicBlock = LLVMGetNextBasicBlock(basicBlock)) {

            for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock);
                instruction;
                instruction = LLVMGetNextInstruction(instruction)) {

                LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);

                if (instruc_opcode == LLVMAlloca) {
                    *local_mem += 4;
                    int to_add = -1 * (*local_mem);
                    offset_map.insert(pair<LLVMValueRef, int> (instruction, to_add));
                }
                else if (instruc_opcode == LLVMStore) {
                    LLVMValueRef first_op = LLVMGetOperand(instruction, 0);
                    LLVMValueRef second_op = LLVMGetOperand(instruction, 1);
                    if (first_op == param) {
                        int first_mem = offset_map.at(first_op);
                        // delete if necessary
                        if (offset_map.count(second_op)) {
                            offset_map.erase(second_op);
                        }
                        offset_map.insert(pair<LLVMValueRef, int> (second_op, first_mem));
                    } else {
                        int second_mem = offset_map.at(second_op);
                        // delete if necessary
                        if (offset_map.count(first_op)) {
                            offset_map.erase(first_op);
                        }
                        offset_map.insert(pair<LLVMValueRef, int> (first_op, second_mem));
                    }
                }
                else if (instruc_opcode == LLVMLoad) {
                    LLVMValueRef first_op = LLVMGetOperand(instruction, 0);
                    int first_mem = offset_map.at(first_op);
                    offset_map.insert(pair<LLVMValueRef, int> (instruction, first_mem));
                }
                
            }
            
        }
    }

    return offset_map;
    
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
            // check if V is in reg_map and not -1 (not -1ed)
            if (reg_map.count(V) && reg_map.at(V) != -1) {
                return V;
            }
        }
    }

    return NULL;
}

const char* get_reg_name(int reg) {
    switch (reg) {
        case EAX: {
            return "eax";
        }
        case EBX: {
            return "ebx";
        }
        case ECX: {
            return "ecx";
        }
        case EDX: {
            return "edx";
        }
        default: {
            return "";
        }
    }
}

const char* get_operator(LLVMOpcode op) {
    switch (op) {
        case (LLVMAdd): {
            return "addl";
        }
        case (LLVMMul): {
            return "imull";
        }
        case (LLVMSub): {
            return "subl";
        }
        default: {return "";}
    }
}