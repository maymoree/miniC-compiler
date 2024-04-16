%{
#include "./ast/ast.h"
#include<stdio.h>
extern int yylex();
extern int yylex_destroy();
extern int yywrap();
int yyerror(char *);
extern FILE * yyin;
%}

%union{
	int ival;
	char * idname;
	astNode *nptr;
	vector<astNode *> *svec_ptr;
}


%token <ival> NUM
%token <idname> NAME
%token PRINT READ RETURN IF ELSE WHILE INT VOID EXTERN
%token LE GE EQ NEQ

%type <nptr> program print_line read_line func code_block
%type <nptr> decl stmt
%type <nptr> condition_block while_block call_func_stmt return_stmt assign_stmt
%type <nptr> if_condition else_condition bool_condition expr term 

%type <svec_ptr> stmts var_decls

%start program

%%
// HAVE TO CHECK FOR FREEs --------------------!!!!!!!

program: print_line read_line func            {$$ = createProg($1, $2, $3);  printNode($$);}
        | read_line print_line func           {$$ = createProg($2, $1, $3); printNode($$);}  // do we need to check
print_line: EXTERN VOID PRINT '(' INT ')' ';'       {$$ = createExtern("print"); printNode($$);}
read_line: EXTERN INT READ '(' ')' ';'              {$$ = createExtern("read"); printNode($$);}



func: INT NAME '(' INT NAME ')' code_block        {$$ = createFunc($2, createVar($5), $7); printNode($$);}     // takes two types, and optional parameter
        | VOID NAME '(' INT NAME ')' code_block   {$$ = createFunc($2, createVar($5), $7); printNode($$);}
        | INT NAME '(' ')' code_block             {$$ = createFunc($2, NULL, $5); printNode($$);}
        | VOID NAME '(' ')' code_block            {$$ = createFunc($2, NULL, $5); printNode($$);}



code_block : '{' var_decls stmts '}' 	{
											vector<astNode*>* new_vec = new vector<astNode*>();
											new_vec->insert(new_vec->end(), $2->begin(), $2->end());
											new_vec->insert(new_vec->end(), $3->begin(), $3->end());
											$$ = createBlock(new_vec); 
											printNode($$);
											delete($2);
											delete($3);
										} 
				| var_decls stmts 	{
											vector<astNode*>* new_vec = new vector<astNode*>();
											new_vec->insert(new_vec->end(), $1->begin(), $1->end());
											new_vec->insert(new_vec->end(), $2->begin(), $2->end());
											$$ = createBlock(new_vec); 
											printNode($$);
											delete($1);
											delete($2);
										} 
				| '{' stmts '}' 		{$$ = createBlock($2); printNode($$);} 
				| stmts					{$$ = createBlock($1); printNode($$);} 



var_decls: var_decls decl 				{$$ = $1; $$->push_back($2);}
				| decl 					{$$ = new vector<astNode*>(); $$->push_back($1);}
decl: INT NAME ';' 						{$$ = createDecl($2);} // do we have to FREE???? ----------!!!!



stmts: stmts stmt 						{$$ = $1; $$->push_back($2);}
		| stmt 							{$$ = new vector<astNode*>(); $$->push_back($1);}
stmt: condition_block					{$$ = $1; printNode($$);}
		| while_block					{$$ = $1; printNode($$);}
		| assign_stmt					{$$ = $1; printNode($$);}
		| call_func_stmt				{$$ = $1; printNode($$);}
		| return_stmt					{$$ = $1; printNode($$);}


condition_block: if_condition code_block else_condition		{$$ = createIf($1, $2, $3);}
					| if_condition code_block				{$$ = createIf($1, $2);}     // handles cases with & w/o {}
if_condition: IF '(' bool_condition ')'			{$$ = $3;}
else_condition: ELSE code_block					{$$ = $2;}


while_block: WHILE '(' bool_condition ')'		{$$ = $3;}


bool_condition: term '<' term				{$$ = createRExpr($1, $3, lt);}
				| term '>' term 			{$$ = createRExpr($1, $3, gt);}
				| term LE term 				{$$ = createRExpr($1, $3, le);}
				| term GE term 				{$$ = createRExpr($1, $3, ge);}
				| term EQ term 				{$$ = createRExpr($1, $3, eq);}
				| term NEQ term 			{$$ = createRExpr($1, $3, neq);}


assign_stmt: NAME '=' expr ';'					{$$ = createAsgn(createVar($1), $3);}
				| NAME '=' call_func_stmt ';'	{$$ = createAsgn(createVar($1), $3);}
expr: term '+' term 							{$$ = createBExpr($1, $3, add);}
		| term '-' term 						{$$ = createBExpr($1, $3, sub);}
		| term '*' term 						{$$ = createBExpr($1, $3, mul);}
		| term '/' term 						{$$ = createBExpr($1, $3, divide);}
		| term 									{$$ = $1;}


term: NUM 										{$$ = createCnst($1);}
		| NAME 									{$$ = createVar($1);}
		| '-' term 								{$$ = createUExpr($2, uminus);}


call_func_stmt: PRINT '(' term ')'		{$$ = createCall("print");}
					| READ '(' ')'		{$$ = createCall("read");}


return_stmt: RETURN '(' expr ')' ';'		{$$ = createRet($3);}
				| RETURN expr ';'			{$$ = createRet($2);}
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