%{
#include "./ast/ast.h"
#include<stdio.h>
extern int yylex();
extern int yylex_destroy();
extern int yywrap();
int yyerror(char *);
extern FILE * yyin;
%}

%union {
    int ival;
    char* sname;
    astNode* node_ptr;
}

%token <ival> NUM
%token <sname> NAME
%token PRINT READ RETURN IF ELSE WHILE INT VOID EXTERN
%token LE GE EQ NEQ
%type <node_ptr> print_line read_line func program
%start program

%%
program: print_line read_line func            {$$ = createProg($1, $2, $3); printf("DONE\n");}
        | read_line print_line func           {$$ = createProg($2, $1, $3);}  // do we need to check

print_line: EXTERN VOID PRINT '(' INT ')' ';'       {$$ = createExtern("print");}
read_line: EXTERN INT READ '(' ')' ';'              {$$ = createExtern("read");}

func: INT NAME '(' INT NAME ')' '{' '}'        {$$ = createFunc($2, createVar($5), NULL);}     // takes two types, and optional parameter
        | VOID NAME '(' INT NAME ')' '{' '}'   {$$ = createFunc($2, createVar($5), NULL);}
        | INT NAME '(' ')' '{' '}'             {$$ = createFunc($2, NULL, NULL);}
        | VOID NAME '(' ')' '{' '}'            {$$ = createFunc($2, NULL, NULL);}
%%
int yyerror(char *s){
	fprintf(stderr,"%s\n", s);
	exit(1);
}

int main(int argc, char* argv[]){
		if (argc == 2){
			yyin = fopen(argv[1], "r");
			if (yyin == NULL) {
				fprintf(stderr, "File open error\n");
				return 1;
			}
		}
		yyparse();
		if (argc == 2) fclose(yyin);
		yylex_destroy();
		return 0;
}