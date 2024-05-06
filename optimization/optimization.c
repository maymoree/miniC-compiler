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
void compute_in_out (LLVMModuleRef module, 
					unordered_map<LLVMBasicBlockRef, std::set<LLVMValueRef>*>* in_map, 
					unordered_map<LLVMBasicBlockRef, std::set<LLVMValueRef>*>* out_map, 
					unordered_map<LLVMBasicBlockRef, std::set<LLVMValueRef>*>* gen_map, 
					unordered_map<LLVMBasicBlockRef, std::set<LLVMValueRef>*>* kill_map);
bool delete_load(LLVMModuleRef module,
				unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* in_map,
				unordered_map<LLVMBasicBlockRef,set<LLVMValueRef>*>* kill_map);
bool local_constant_folding(LLVMModuleRef module);
bool global_constant_propagation(LLVMModuleRef module);
void optimize(LLVMModuleRef module);
void main_optimization(char* file);

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


					else if (instruc_opcode == LLVMAdd || instruc_opcode == LLVMMul) {
						
						LLVMValueRef operand1 = LLVMGetOperand(instruction, 0);
						LLVMValueRef operand2 = LLVMGetOperand(instruction, 1);

						// loop through all other instructions
						for (LLVMValueRef other_instruc = LLVMGetNextInstruction(instruction); 
						other_instruc; 
						other_instruc = LLVMGetNextInstruction(other_instruc)) {

							if ((instruc_opcode == LLVMGetInstructionOpcode(other_instruc)) &&
							 (((operand1 == LLVMGetOperand(other_instruc,1)) && (operand2 == LLVMGetOperand(other_instruc,0))) || 
							 ((operand1 == LLVMGetOperand(other_instruc,0)) && (operand2 == LLVMGetOperand(other_instruc,1))))) {
							
							// replace B with A
							elim_instructions->push_back(other_instruc);
							LLVMReplaceAllUsesWith(other_instruc, instruction);
							}	
						}

					}

					else if (instruc_opcode == LLVMSub) {

						LLVMValueRef operand1 = LLVMGetOperand(instruction, 0);
						LLVMValueRef operand2 = LLVMGetOperand(instruction, 1);

						// loop through all other instructions
						for (LLVMValueRef other_instruc = LLVMGetNextInstruction(instruction); 
						other_instruc; 
						other_instruc = LLVMGetNextInstruction(other_instruc)) {

							if ((instruc_opcode == LLVMGetInstructionOpcode(other_instruc)) &&
							((operand1 == LLVMGetOperand(other_instruc,1)) && (operand2 == LLVMGetOperand(other_instruc,0)))) {
							
							// replace B with A
							elim_instructions->push_back(other_instruc);
							LLVMReplaceAllUsesWith(other_instruc, instruction);
							}	
						}

					}


				}
		}

	}
	// help_print_instructions(module);
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
	// help_print_instructions(module);
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


				// printf("\nGen Block:\n");
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
			// printf("\nKill Block:\n");
			// set<LLVMValueRef>::iterator itr;
			// for (itr = kill_block->begin(); itr != kill_block->end(); itr++) {
			// 	LLVMDumpValue(*itr);
			// }
			(*kill_map)[basicBlock] = kill_block;
		}
	}
	delete(all_stores);
}

void compute_in_out (LLVMModuleRef module, 
					unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* in_map, 
					unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* out_map, 
					unordered_map<LLVMBasicBlockRef,set<LLVMValueRef>*>* gen_map, 
					unordered_map<LLVMBasicBlockRef,set<LLVMValueRef>*>* kill_map) {

	// initialize new unordered map to predecessors
	unordered_map<LLVMBasicBlockRef, set<LLVMBasicBlockRef>*>* pred_map = new unordered_map<LLVMBasicBlockRef, set<LLVMBasicBlockRef>*> ();
	// create empty set for each block first to store predecessors
	for (LLVMValueRef function = LLVMGetFirstFunction(module); 
		function; 
		function = LLVMGetNextFunction(function)) {

			// loop through basic block
			for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
			basicBlock;
			basicBlock = LLVMGetNextBasicBlock(basicBlock)) {
				
				(*pred_map)[basicBlock] = new set<LLVMBasicBlockRef> ();

			}
	}

	// loop through functions
	for (LLVMValueRef function = LLVMGetFirstFunction(module); 
	function; 
	function = LLVMGetNextFunction(function)) {

		// loop through basic block
		for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
		basicBlock;
		basicBlock = LLVMGetNextBasicBlock(basicBlock)) {
				
			// initialize empty set
			set<LLVMValueRef>* in_block = new set<LLVMValueRef> ();
			(*in_map)[basicBlock] = in_block;

			// set out b to gen b
			set<LLVMValueRef>* gen_block = (*gen_map)[basicBlock];
			set<LLVMValueRef>* out_block = new set<LLVMValueRef>(*gen_block);
			(*out_map)[basicBlock] = out_block;

			// update the predecessor of this block's successors
			LLVMValueRef terminator = LLVMGetBasicBlockTerminator(basicBlock);
            for (int i = 0; i < LLVMGetNumSuccessors(terminator); i++) {
                LLVMBasicBlockRef successor = LLVMGetSuccessor(terminator, i);
                (*pred_map)[successor]->insert(basicBlock);
            }

			// printf("\n For this block: ");
			// LLVMDumpValue(LLVMBasicBlockAsValue(basicBlock));
			// printf("\n");

			// printf("\nIn Block:\n");
			// set<LLVMValueRef>::iterator itr1;
			// for (itr1 = (*in_map)[basicBlock]->begin(); itr1 != (*in_map)[basicBlock]->end(); itr1++) {
			// 	LLVMDumpValue(*itr1);
			// }

			// printf("\nOut Block:\n");
			// set<LLVMValueRef>::iterator itr2;
			// for (itr2 = (*out_map)[basicBlock]->begin(); itr2 != (*out_map)[basicBlock]->end(); itr2++) {
			// 	LLVMDumpValue(*itr2);
			// }

		}

	}

	// set change to true
	bool change = true;

	while (change){
		change = false;

		// keep the old out of the computation before (e.g. OUT0 to use for IN1)
		// have to use this to compute 
		// Initialize old_out_map with deep copies of out_map's sets
		unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* old_out_map = new unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>();

		for (auto& block_out_pair : *out_map) {
			LLVMBasicBlockRef block = block_out_pair.first;
			set<LLVMValueRef>* new_set = new set<LLVMValueRef>(*block_out_pair.second); // Make a new set copying the old one
			(*old_out_map)[block] = new_set;
		}

		// loop through functions
		for (LLVMValueRef function = LLVMGetFirstFunction(module); 
		function; 
		function = LLVMGetNextFunction(function)) {

			// loop through basic block
			for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
			basicBlock;
			basicBlock = LLVMGetNextBasicBlock(basicBlock)) {
				
				// printf("\n For this block: ");
				// LLVMDumpValue(LLVMBasicBlockAsValue(basicBlock));
				// printf("\n");

				// printf("\n OLD out Block:\n");
				// set<LLVMValueRef>::iterator itr0;
				// for (itr0 = (*old_out_map)[basicBlock]->begin(); itr0 != (*old_out_map)[basicBlock]->end(); itr0++) {
				// 	LLVMDumpValue(*itr0);
				// }
				
				// IN[B] = union(OUT[P1], OUT[P2],.., OUT[PN]),
				// where P1, P2, .. PN are predecessors of basic block B in the control flow graph.		
				// HAS TO USE OLD OUT OF P1, P2, ... PN		
				set<LLVMBasicBlockRef>* pred_block = (*pred_map)[basicBlock]; // get predecessors
				// get the union of outs of predecessors; set makes sure no repetition
				set<LLVMBasicBlockRef>::iterator pred;
				for (LLVMBasicBlockRef pred : *pred_block) {
					set<LLVMValueRef>* pred_out_block = (*old_out_map)[pred]; // Get OUT[Pred]

					// printf("\nPRED'S OLD OUT:\n");
					// set<LLVMValueRef>::iterator itr5;
					// for (itr5 = pred_out_block->begin(); itr5 != pred_out_block->end(); itr5++) {
					// 	LLVMDumpValue(*itr5);
					// }

					(*in_map)[basicBlock]->insert(pred_out_block->begin(), pred_out_block->end()); // Add OUT[Pred]
				}

				// printf("\nIn Block:\n");
				// set<LLVMValueRef>::iterator itr1;
				// for (itr1 = (*in_map)[basicBlock]->begin(); itr1 != (*in_map)[basicBlock]->end(); itr1++) {
				// 	LLVMDumpValue(*itr1);
				// }

				// oldout = OUT[B]
				set<LLVMValueRef>* old_out = (*old_out_map)[basicBlock];

				// OUT[B] = GEN[B] union (in[B] - kill[B])
				// get inb - killb
        		set_difference((*in_map)[basicBlock]->begin(), (*in_map)[basicBlock]->end(), 
								(*kill_map)[basicBlock]->begin(), (*kill_map)[basicBlock]->end(), 
								// adds directly into out[b], which is already gen[b]
								// automatically calculates union due to set
								inserter(*(*out_map)[basicBlock],(*out_map)[basicBlock]->end()));

				// if (OUT[B] != oldout) change = True 
				// if ((*out_map)[basicBlock] != old_out) {
				// 	printf("\nCHANGED TO TRUE ----------------------\n");
				// 	change = true;
				// }
				// Check if OUT[B] has changed compared to old_out
				set<LLVMValueRef>* current_out = (*out_map)[basicBlock];
				set<LLVMValueRef>* previous_out = (*old_out_map)[basicBlock];

				if (current_out->size() != previous_out->size() || !std::equal(current_out->begin(), current_out->end(), previous_out->begin())) {
					// printf("\nCHANGED TO TRUE ----------------------\n");
					change = true;
				}

				// printf("\nOut Block:\n");
				// set<LLVMValueRef>::iterator itr2;
				// for (itr2 = (*out_map)[basicBlock]->begin(); itr2 != (*out_map)[basicBlock]->end(); itr2++) {
				// 	LLVMDumpValue(*itr2);
				// }

				// printf("\nTrue: %d\n", change);
			}
		}

		// clean up old out map
		for (auto& block_set_pair : *old_out_map) {
			delete block_set_pair.second;
		}

		delete(old_out_map);

	}
	for (auto pred = pred_map->begin(); pred != pred_map->end(); ++pred) {
		delete pred->second;
	}

	delete(pred_map);
}

bool delete_load(LLVMModuleRef module,
				unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* in_map,
				unordered_map<LLVMBasicBlockRef,set<LLVMValueRef>*>* kill_map) {

	bool changes_made = false;

	// loop through functions
	for (LLVMValueRef function = LLVMGetFirstFunction(module); 
	function; 
	function = LLVMGetNextFunction(function)) {

		// loop through basic block
		for (LLVMBasicBlockRef basicBlock = LLVMGetFirstBasicBlock(function);
		basicBlock;
		basicBlock = LLVMGetNextBasicBlock(basicBlock)) {
				
			// initialize empty set
			// all instrucs in IN are stores
			set<LLVMValueRef>* in_block = (*in_map)[basicBlock];
			set<LLVMValueRef>* R = new set<LLVMValueRef>(*in_block);

			// set to store instructions to delete
			set<LLVMValueRef>* erase_load_instruc = new set<LLVMValueRef> ();

			// printf("\n       NEW BLOCK --------------\n");

			// printf("\nR Block:\n");
			// set<LLVMValueRef>::iterator itr;
			// for (itr = R->begin(); itr != R->end(); itr++) {
			// 	LLVMDumpValue(*itr);
			// }

			for (LLVMValueRef instruction = LLVMGetFirstInstruction(basicBlock);
			instruction;
			instruction = LLVMGetNextInstruction(instruction)) {

				// printf("\nTHIS INSTRUCTION----\n");
				// LLVMDumpValue(instruction);
				// printf("\n");

				LLVMOpcode instruc_opcode = LLVMGetInstructionOpcode(instruction);

				// instruction is store
				if (instruc_opcode == LLVMStore) {

					// printf("\nSTORE\n");
					
					// add instruction
					R->insert(instruction);

					// remove instructions' kill_set from R
					LLVMValueRef instruc_operand = LLVMGetOperand(instruction,1); // get locatioin of instruction
					set<LLVMValueRef>::iterator killed_instruc;
					for (killed_instruc = R->begin(); killed_instruc != R->end(); ) {
						// if it's not itself, and the same memory --> gets killed
						if ((*killed_instruc != instruction) && (LLVMGetOperand(*killed_instruc,1) == instruc_operand)) {
								killed_instruc = R->erase(killed_instruc); // update killed_instruc
						} else {
							++killed_instruc;
						}
					}

					// printf("\nR Block UPDATED:\n");
					// set<LLVMValueRef>::iterator itr;
					// for (itr = R->begin(); itr != R->end(); itr++) {
					// 	LLVMDumpValue(*itr);
					// }
				}

				// instruction is load
				if (instruc_opcode == LLVMLoad) {

					// printf("\nLOAD\n");

					// get location of instruction, %t
					LLVMValueRef instruc_operand = LLVMGetOperand(instruction,0);

					LLVMValueRef last_r = NULL; // grabs the last constant value

					bool all_const = true;

					set<LLVMValueRef>* r_const = new set<LLVMValueRef> ();

					// check if all instrucs that store to %t are constant
					set<LLVMValueRef>::iterator r_instruc;
					for (r_instruc = R->begin(); r_instruc != R->end(); ++r_instruc){
						// if it at the same location
						if (instruc_operand == LLVMGetOperand(*r_instruc, 1)) {
							// check if it is a constant
							if (!LLVMIsConstant(LLVMGetOperand(*r_instruc, 0))) {
								// printf("\n NOT A CONSTANT \n");
								all_const = false;
							}
							r_const->insert(*r_instruc); // still store to r_const 
							last_r = *r_instruc; // still get the last const in all R that's at loc %t
						}
					}
					
					// use r_const in here; else delete immediately
					if (last_r != NULL && all_const) {

						// printf("\n NOT NULL !!\n");
						// printf("This is last r: \n");
						// LLVMDumpValue(last_r);

						// check if all values are the same constant
						bool same_const = true;
						for (r_instruc = r_const->begin(); r_instruc != r_const->end(); r_instruc++) {
							// should we compare the one next to it? or would that be messy : (
							// check if their constant values are the same
							if (LLVMConstIntGetSExtValue(LLVMGetOperand(*r_instruc, 0)) != LLVMConstIntGetSExtValue(LLVMGetOperand(last_r, 0))) {
								same_const = false;
								// printf("\nNOT SAME CONSTANT\n");
							}
						}

						// replace all uses of instruction with constant
						if (same_const) {
							// printf("\nALL SAME CONSTANT\n");
							long long target_const = LLVMConstIntGetSExtValue(LLVMGetOperand(last_r, 0));
							LLVMReplaceAllUsesWith(instruction, LLVMConstInt(LLVMInt32Type(), target_const, 0));
							erase_load_instruc->insert(instruction);
						}

					}

					delete(r_const);
				}
			}

			// determining if any changes were made in constant propagation
			if (erase_load_instruc->size() > 0) {
				changes_made = true;
			}

			set<LLVMValueRef>::iterator instruc;
			for (instruc = erase_load_instruc->begin(); instruc != erase_load_instruc->end(); ++instruc){
				LLVMInstructionEraseFromParent(*instruc);
			}

			delete(erase_load_instruc);
			delete(R);

		}
	}
    return changes_made;
	// help_print_instructions(module);
}





// -------------------------------------------------------
// OPTIMIZATION FUNCTIONS TOGETHER -----------------------
// AND CLEAN UPS -----------------------------------------
// -------------------------------------------------------

bool local_constant_folding(LLVMModuleRef module) {

	bool changes_made = false;

	vector<LLVMValueRef>* common_elim = common_sub_expr(module);

	dead_code_elim(module, common_elim);

	vector<LLVMValueRef>* const_elim = const_folding(module);

	dead_code_elim(module, const_elim);

	if (const_elim->size() > 0){
		// changes made if there are more code to eliminate
		changes_made = true;
	}

	// clean up
	delete(common_elim);
	delete(const_elim);

	return changes_made;

}

bool global_constant_propagation(LLVMModuleRef module) {
	
	// create unordered maps
	unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* gen_map = new unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*> ();
	unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* kill_map = new unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*> ();
	unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* in_map = new unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*> ();
	unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*>* out_map = new unordered_map<LLVMBasicBlockRef, set<LLVMValueRef>*> ();
	
	// call the functions
	compute_gen(module, gen_map);
	// help_print_instructions(module);

	compute_kill(module, kill_map);
	// help_print_instructions(module);

	compute_in_out(module, in_map, out_map, gen_map, kill_map);
	// help_print_instructions(module);

	bool changes_made = delete_load(module, in_map, kill_map);
	// help_print_instructions(module);

	// cleanup
	for (auto set_gen = gen_map->begin(); set_gen != gen_map->end(); ++set_gen) {
		delete set_gen->second;
	}
	delete(gen_map);

	for (auto set_kill = kill_map->begin(); set_kill != kill_map->end(); ++set_kill) {
		delete set_kill->second;
	}
	delete(kill_map);

	for (auto set_in = in_map->begin(); set_in != in_map->end(); ++set_in) {
		delete set_in->second;
	}
	delete(in_map);

	for (auto set_out = out_map->begin(); set_out != out_map->end(); ++set_out) {
		delete set_out->second;
	}
	delete(out_map);

	// either returns true or false
	return changes_made;
}

void optimize(LLVMModuleRef module) {

	bool changes_made = true;

	while (changes_made) {
		changes_made = false;

		changes_made = global_constant_propagation(module);
		changes_made = local_constant_folding(module);

		// help_print_instructions(module);
	}
	help_print_instructions(module);
	printf("\n ------------------------- OPTIMIZED FIXED POINT ------------------------- \n");
}



// HELPER FUNCTIONS!!!!! ---------------------------------

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
				
				printf("\nList of Instructions------------\n");

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

// call main_optimization in outside main
void main_optimization(char* file) {

	LLVMModuleRef m;

	m = createLLVMModel(file);

	if (m != NULL){

		optimize(m);

		LLVMPrintModuleToFile (m, "test.ll", NULL);

		LLVMDisposeModule(m);
	}
	else {
	    fprintf(stderr, "m is NULL\n");
	}
	LLVMShutdown();

}