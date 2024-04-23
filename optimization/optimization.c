#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>
#include <llvm-c/Types.h>

#define prt(x) if(x) { printf("%s\n", x); }

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
						printf("\n");
						printf("\nINSTRUCTION: \n");
						LLVMDumpValue(instruction);
						printf("\n");


						LLVMValueRef instruc_operand = LLVMGetOperand(instruction, 0);
						
						// check if there is store in between A and B
						bool found_store = false;

						// loop through all instructions to compare to A (these are B)
						for (LLVMValueRef other_instruc = LLVMGetNextInstruction(instruction); 
						other_instruc; 
						other_instruc = LLVMGetNextInstruction(other_instruc)) {

							printf("\n-----Other Instruction: \n");
							LLVMDumpValue(other_instruc);
							printf("\n");

							// if opcode is store, there is possbile modification of B
							if ((LLVMGetInstructionOpcode(other_instruc) == LLVMStore) 
							&& (LLVMGetOperand(other_instruc,1) == instruc_operand)) {
								printf("\n---------STORE: \n");
								LLVMDumpValue(other_instruc);
								printf("\n");
								found_store = true;
								continue;
							}

							// if opcode is load
							else if (LLVMGetInstructionOpcode(other_instruc) == instruc_opcode
							&& LLVMGetOperand(other_instruc, 0) == instruc_operand
							&& !found_store) {
								printf("\n---------LOAD REPLACED: \n");
								LLVMDumpValue(other_instruc);
								printf("\n");
								// replace B with A
								LLVMReplaceAllUsesWith(other_instruc, instruction);
							}
							continue;

						}	
					}
				}
		}

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
		//LLVMDumpModule(m);
		// walkGlobalValues(m);
		// walkFunctions(m);
		// LLVMPrintModuleToFile (m, "test_new.ll", NULL);

	}
	else {
	    fprintf(stderr, "m is NULL\n");
	}
	
	return 0;
}
