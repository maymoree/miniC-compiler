#include <ast.h>
#include <smta.h>
#include <optimization.h>
#include <irbuilder.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
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

    if (semantic_analysis(root)){
        printf("Passed semantic analysis.\n");
    } else {
        printf("Failed semantic analysis.\n");
        return 1;
    }

    // PART 2 --------------------
    printf("\nPART 2 !!!!!!!\n");
    LLVMModuleRef mod = ir_builder(root);

    // PART 3 ---------------------
    if (argc >= 2){
        main_optimization(argv[2]);
    }

    // clean up
    if (argc >= 2) fclose(yyin);
    yylex_destroy();
    freeNode(root);
    return 0;
}