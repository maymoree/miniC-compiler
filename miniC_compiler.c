#include <ast.h>
#include <smta.h>
#include <optimization.h>
#include <irbuilder.h>
#include <generator.h>

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

using namespace std;


int main(int argc, char* argv[]){
    // PART 1 --------------------
    // takes the second argument for front end part

    printf("\n------------------------------------ PART 1 !!!!!!! ------------------------------------\n");
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
    printf("\n------------------------------------ PART 2 !!!!!!! ------------------------------------\n");
    LLVMModuleRef mod = ir_builder(root);


    // PART 3 ---------------------
    printf("\n------------------------------------ PART 3 !!!!!!! ------------------------------------\n");
    main_optimization(mod);

    printf("\n------------------------------------ PART 4 !!!!!!! ------------------------------------\n");
    generator(mod);

    // clean up
    if (argc >= 2) fclose(yyin);
    yylex_destroy();
    LLVMDisposeModule(mod); // ???
    freeNode(root);
    return 0;
}