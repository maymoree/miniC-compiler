#include <ast.h> // -I include paths
#include "smta.h" // local folder
#include <stdio.h>
extern int yylex();
extern int yylex_destroy();
extern int yywrap();
int yyerror(char *);
extern FILE * yyin;
extern int yyparse();
astNode* ret_root();

int main(int argc, char* argv[]){
		#if YYDEBUG
			 yydebug = 1;
		#endif

		if (argc == 2){
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
		}

		freeNode(root);
		
		if (argc == 2) fclose(yyin);
		yylex_destroy();
		return 0;
}