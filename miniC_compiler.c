#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "./ast/ast.h"
#include "./frontend/smta.h"
#include "./optimization/optimization.h"

#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>
#include <llvm-c/Types.h>

extern int yylex();
extern int yywrap();
extern int yylineno;
extern int yyparse();
extern int yylex_destroy();
extern FILE *yyin;

extern astNode* ret_root();

int main(int argc, char* argv[]){

		// PART 1 --------------------

		// takes the second argument for front end part
		if (argc >= 2){

			yyin = fopen(argv[1], "r");

			if (yyin == NULL) {
				fprintf(stderr, "File open error\n");
				return 1;
			}

		}

		yyparse();

        astNode* root = ret_root();

		// prints the ast
		if (semantic_analysis(root)){
			printf("Passed semantic analysis.\n");
		} else {
			printf("Failed semantic analysis.\n");
			return 1;
		}

		freeNode(root);


		
		
		// PART 3 ---------------------

		LLVMModuleRef m;

		if (argc >= 2){
			m = createLLVMModel(argv[2]);
		}
		else{
			m = NULL;
			return 1;
		}

		main_optimization(m);
		

		
		
		// clean up
		if (argc >= 2) fclose(yyin);
		yylex_destroy();

		return 0;
}