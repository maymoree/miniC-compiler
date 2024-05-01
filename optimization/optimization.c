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

vector<LLVMValueRef>* common_sub_expr(LLVMModuleRef module) {

	vector<LLVMValueRef>* elim_instructions = new vector<LLVMValueRef> ();

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
	return (elim_instructions);

}

void dead_code_elim(LLVMModuleRef module, vector<LLVMValueRef>* elim_instructions){
	
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
							// //get next instruction first before deleting the current one
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

vector<LLVMValueRef>* const_folding (LLVMModuleRef module) {

	vector<LLVMValueRef>* elim_instructions = new vector<LLVMValueRef> ();

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

					LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);

					// binary operators
					if (instruc_opcode == LLVMAdd || instruc_opcode == LLVMSub || instruc_opcode == LLVMMul) {
						LLVMValueRef operand1 = LLVMGetOperand(instruction, 0);
						LLVMValueRef operand2 = LLVMGetOperand(instruction, 1);

						// check if both operands are constants
						if (LLVMIsConstant(operand1) && LLVMIsConstant(operand2)){
							if (instruc_opcode == LLVMAdd) {
								int counter = count(elim_instructions->begin(), elim_instructions->end(), instruction);
								if (counter <= 0) {elim_instructions->push_back(instruction);}
								LLVMValueRef added;
								added = LLVMConstAdd(operand1, operand2);
								LLVMReplaceAllUsesWith(instruction, added);

							} else if (instruc_opcode == LLVMSub) {
								int counter = count(elim_instructions->begin(), elim_instructions->end(), instruction);
								if (counter <= 0) {elim_instructions->push_back(instruction);}
								LLVMReplaceAllUsesWith(instruction, LLVMConstSub(operand1, operand2));

							} else if (instruc_opcode == LLVMMul) {
								int counter = count(elim_instructions->begin(), elim_instructions->end(), instruction);
								if (counter <= 0) {elim_instructions->push_back(instruction);}
								LLVMReplaceAllUsesWith(instruction, LLVMConstMul(operand1, operand2));
							}
						}


					}
					
				}

			}

	}
	return(elim_instructions);
}

void compute_gen(LLVMModuleRef module, unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* gen_map) {

	// loop through functions
	for (LLVMValueRef function = LLVMGetFirstFunction(module); 
	function; 
	function = LLVMGetNextFunction(function)) {

		// loop through basic block
		for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
 			 basicBlock;
  			 basicBlock = LLVMGetNextBasicBlock(basicBlock)) {
				
				// initialize empty set
				set<LLVMValueRef>* gen_block = new set<LLVMValueRef> ();

				// loop through instructions in each block
				for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock); 
				instruction; 
				instruction = LLVMGetNextInstruction(instruction)) {

					LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);

					// check if opcode is store instruction
					if (instruc_opcode == LLVMStore) {

						// printf("STORE FOUND\n");

						// check to remove killed instructions
						LLVMValueRef operand = LLVMGetOperand(instruction, 1); // gets location
						// set to store instructions that have to be deleleted
						set<LLVMValueRef>* to_erase = new set<LLVMValueRef> ();
						// loops through and checks for instructions
						set<LLVMValueRef>::iterator other_instruc;
						for (other_instruc = gen_block->begin(); other_instruc != gen_block->end(); ++other_instruc) {
							if (operand == LLVMGetOperand(*other_instruc, 1)) {
								to_erase->insert(*other_instruc);
							}
						}
						// erases those instructions from gen_block
						set<LLVMValueRef>::iterator erase_instruc;
						for (erase_instruc = to_erase->begin(); erase_instruc != to_erase->end(); ++erase_instruc){
							gen_block->erase(*erase_instruc);
						}
						// delete the created set
						delete(to_erase);

						// add instruction to gen_block
						gen_block->insert(instruction);

					}

				}

				// insert block's set into unordered map
				(*gen_map)[basicBlock] = gen_block;


				// printf("Gen Block:\n");
				// set<LLVMValueRef>::iterator itr;
				// for (itr = gen_block->begin(); itr != gen_block->end(); itr++) {
				// 	LLVMDumpValue(*itr);
				// }
		}
	}

}

void compute_kill(LLVMModuleRef module, unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* kill_map) {
	
	// get all stores
	set<LLVMValueRef>* all_stores = find_all_stores(module);

	// loop through functions
	for (LLVMValueRef function = LLVMGetFirstFunction(module); 
	function; 
	function = LLVMGetNextFunction(function)) {

		// loop through basic block
		for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
		basicBlock;
		basicBlock = LLVMGetNextBasicBlock(basicBlock)) {
				
			// initialize empty set
			set<LLVMValueRef>* kill_block = new set<LLVMValueRef> ();

			for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock);
			instruction;
			instruction = LLVMGetNextInstruction(instruction)) {

				LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);

				// check if opcode is store instruction
				if (instruc_opcode == LLVMStore) {

					LLVMValueRef operand = LLVMGetOperand(instruction, 1); // gets location

					set<LLVMValueRef>::iterator store_instruc;
					for (store_instruc = all_stores->begin(); store_instruc != all_stores->end(); ++store_instruc){
						
						if (operand == LLVMGetOperand(*store_instruc, 1) && *store_instruc != instruction) {
							kill_block->insert(*store_instruc);
						}

					}

				}

			}
			(*kill_map)[basicBlock] = kill_block;
		}
	}
	delete(all_stores);
}






// HELPER FUNCTIONS!!!!!

set<LLVMValueRef>* find_all_stores(LLVMModuleRef module) {
	
	// initialize empty set
	set<LLVMValueRef>* all_stores = new set<LLVMValueRef> ();

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

					if (LLVMGetInstructionOpcode(instruction) == LLVMStore) {
						all_stores->insert(instruction);
					}

				}
		}
	}
	return all_stores;

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
		vector<LLVMValueRef>* common_elim = common_sub_expr(m);
		dead_code_elim(m, common_elim);
		vector<LLVMValueRef>* const_elim = const_folding(m);
		dead_code_elim(m, const_elim);

		unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* gen_map = new unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*> ();
		compute_gen(m, gen_map);

		unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* kill_map = new unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*> ();
		compute_kill(m, kill_map);

		LLVMDisposeModule(m);
		delete(common_elim);
		delete(const_elim);

		for (auto set_gen = gen_map->begin(); set_gen != gen_map->end(); ++set_gen) {
    		delete set_gen->second;
		}

		delete(gen_map);

		for (auto set_kill = kill_map->begin(); set_kill != kill_map->end(); ++set_kill) {
    		delete set_kill->second;
		}

		delete(kill_map);
	}
	else {
	    fprintf(stderr, "m is NULL\n");
	}
	LLVMShutdown();

	return 0;
}
