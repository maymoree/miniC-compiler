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

/* This function reads the given llvm file and loads the LLVM IR into
	 data-structures that we can works on for optimization phase.
*/
 
LLVMModuleRef createLLVMModel(char * filename){
	char *err = 0;

	LLVMMemoryBufferRef ll_f = 0;
	LLVMModuleRef m = 0;

	LLVMCreateMemoryBufferWithContentsOfFile(filename, &ll_f, &err);

	if (err != NULL) { 
		prt(err);
		return NULL;
	}
	
	LLVMParseIRInContext(LLVMGetGlobalContext(), ll_f, &m, &err);

	if (err != NULL) {
		prt(err);
	}

	return m;
}

void common_sub_expr(LLVMModuleRef module) {

	// loop through functions
	for (LLVMValueRef function = LLVMGetFirstFunction(module); 
	function; 
	function = LLVMGetNextFunction(function)) {

		// loop through basic block
		for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
 			 basicBlock;
  			 basicBlock = LLVMGetNextBasicBlock(basicBlock)) {

				// loop through instructions in each block
				for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock); 
				instruction; 
				instruction = LLVMGetNextInstruction(instruction)) {

					// get opcode
					LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);

					// check for similarity only if A is a load instruction
					if (instruc_opcode == LLVMLoad) {

						// can this just be instruction?? same type of value
						LLVMValueRef instruc_operand = LLVMGetOperand(instruction, 0);
						
						// check if there is store in between A and B
						bool found_store = false;

						// loop through all instructions to compare to A (these are B)
						for (LLVMValueRef other_instruc = LLVMGetNextInstruction(instruction); 
						other_instruc; 
						other_instruc = LLVMGetNextInstruction(other_instruc)) {

							// if opcode is store, there is possbile modification of B
							if ((LLVMGetInstructionOpcode(other_instruc) == LLVMStore) 
							&& (LLVMGetOperand(other_instruc,1) == instruc_operand)) {

								found_store = true;
								continue;
							}

							// if opcode is load
							else if (LLVMGetInstructionOpcode(other_instruc) == instruc_opcode
							&& LLVMGetOperand(other_instruc, 0) == instruc_operand
							&& !found_store) {

								// replace B with A
								int counter = count(elim_instructions->begin(), elim_instructions->end(), other_instruc);
								if (counter <= 0) {elim_instructions->push_back(other_instruc);}
								LLVMReplaceAllUsesWith(other_instruc, instruction);
							}
							continue;

						}	
					}
				}
		}

	}

}

void dead_code_elim(LLVMModuleRef module){
	
	// loop through functions
	for (LLVMValueRef function = LLVMGetFirstFunction(module); 
	function; 
	function = LLVMGetNextFunction(function)) {

		// loop through basic block
		for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
 			 basicBlock;
  			 basicBlock = LLVMGetNextBasicBlock(basicBlock)) {

				// loop through all instructions
				for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock); instruction;) {

					LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);

					if ((instruc_opcode != LLVMAlloca) && (instruc_opcode != LLVMStore) 
					&& (instruc_opcode != LLVMBr) && (instruc_opcode != LLVMRet) && (instruc_opcode != LLVMCall)) {
						
						// check if instruction is in instructions to be eliminated
						int counter = count(elim_instructions->begin(), elim_instructions->end(), instruction);

						// if found, eliminate it
						if (counter > 0) {
							LLVMValueRef elim_instruction = instruction;
							//get next instruction first before deleting the current one
							instruction = LLVMGetNextInstruction(instruction);
							LLVMInstructionEraseFromParent(elim_instruction);
						} else {
							instruction = LLVMGetNextInstruction(instruction);
						}

					} else {
						instruction = LLVMGetNextInstruction(instruction);
					}
				
				}

								

			}

	}
	help_print_instructions(module);
}



void help_print_instructions(LLVMModuleRef module) {
	// loop through functions
	for (LLVMValueRef function = LLVMGetFirstFunction(module); 
	function; 
	function = LLVMGetNextFunction(function)) {

		// loop through basic block
		for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
 			 basicBlock;
  			 basicBlock = LLVMGetNextBasicBlock(basicBlock)) {
				
				printf("List of Instructions------------\n");

				// loop through instructions in each block
				for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock); 
				instruction; 
				instruction = LLVMGetNextInstruction(instruction)) {
					printf("\n");
					LLVMDumpValue(instruction);
					printf("\n");
				}

			}

	}
}

void print_vector(vector<LLVMValueRef>* elim_instruction) {
	for (int i = 0; i < elim_instruction->size(); i++){
			LLVMDumpValue(elim_instruction->at(i));
        }
}


int main(int argc, char** argv)
{
	LLVMModuleRef m;

	if (argc == 2){
		m = createLLVMModel(argv[1]);
	}
	else{
		m = NULL;
		return 1;
	}

	if (m != NULL){
		common_sub_expr(m);
		dead_code_elim(m);

	}
	else {
	    fprintf(stderr, "m is NULL\n");
	}
	
	return 0;
}
