%{
#include <ast.h>
#include "smta.h"
#include<stdio.h>
extern int yylex();
extern int yylex_destroy();
extern int yywrap();
int yyerror(char *);
extern FILE * yyin;

astNode* root;
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

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%start program
%%

program: print_line read_line func            {$$ = createProg($1, $2, $3);
												root = $$;}
print_line: EXTERN VOID PRINT '(' INT ')' ';'       {$$ = createExtern("print");}
read_line: EXTERN INT READ '(' ')' ';'              {$$ = createExtern("read");}



func: INT NAME '(' INT NAME ')' code_block        {$$ = createFunc($2, createVar($5), $7);
													free($2); free($5);}     // takes two types, and optional parameter
        | VOID NAME '(' INT NAME ')' code_block   {$$ = createFunc($2, createVar($5), $7);
													free($2); free($5);}
        | INT NAME '(' ')' code_block             {$$ = createFunc($2, NULL, $5);
													free($2);}
        | VOID NAME '(' ')' code_block            {$$ = createFunc($2, NULL, $5);
													free($2);}



code_block : '{' var_decls stmts '}' 	{
											vector<astNode*>* new_vec = new vector<astNode*>();
											new_vec->insert(new_vec->end(), $2->begin(), $2->end());
											new_vec->insert(new_vec->end(), $3->begin(), $3->end());
											$$ = createBlock(new_vec); 
											delete($2);
											delete($3);
										} 
				| '{' stmts '}' 		{$$ = createBlock($2);} 


stmts: stmts stmt 						{$$ = $1; $$->push_back($2);}
		| stmt 							{$$ = new vector<astNode*>(); $$->push_back($1);}
stmt: assign_stmt						{$$ = $1;}
		| call_func_stmt				{$$ = $1;}
		| return_stmt					{$$ = $1;}
		| condition_block				{$$ = $1;}
		| while_block					{$$ = $1;}
		| code_block					{$$ = $1;}


condition_block: if_condition stmt %prec LOWER_THAN_ELSE				{$$ = createIf($1, $2);}     // handles cases with & w/o {}
					| if_condition stmt else_condition				{$$ = createIf($1, $2, $3);}
if_condition: IF '(' bool_condition ')'			{$$ = $3;}
else_condition: ELSE stmt						{$$ = $2;}


while_block: WHILE '(' bool_condition ')' stmt				{$$ = createWhile($3,$5);}


bool_condition: term '<' term				{$$ = createRExpr($1, $3, lt);}
				| term '>' term 			{$$ = createRExpr($1, $3, gt);}
				| term LE term 				{$$ = createRExpr($1, $3, le);}
				| term GE term 				{$$ = createRExpr($1, $3, ge);}
				| term EQ term 				{$$ = createRExpr($1, $3, eq);}
				| term NEQ term 			{$$ = createRExpr($1, $3, neq);}


assign_stmt: NAME '=' expr ';'					{$$ = createAsgn(createVar($1), $3);
												free($1);}
				| NAME '=' call_func_stmt		{$$ = createAsgn(createVar($1), $3);
												free($1);}
expr: term '+' term 							{$$ = createBExpr($1, $3, add);}
		| term '-' term 						{$$ = createBExpr($1, $3, sub);}
		| term '*' term 						{$$ = createBExpr($1, $3, mul);}
		| term '/' term 						{$$ = createBExpr($1, $3, divide);}
		| term 									{$$ = $1;}


term: NUM 										{$$ = createCnst($1);}
		| NAME 									{$$ = createVar($1); free($1);}
		| '-' term 								{$$ = createUExpr($2, uminus);}


var_decls: var_decls decl 				{$$ = $1; $$->push_back($2);}
				| decl 					{$$ = new vector<astNode*>(); $$->push_back($1);}
decl: INT NAME ';' 						{$$ = createDecl($2);
										free($2);}


call_func_stmt: PRINT '(' NAME ')' ';'	{$$ = createCall("print", createVar($3)); free($3);}
					| READ '(' ')' ';' 	{$$ = createCall("read");}


return_stmt: RETURN '(' expr ')' ';'		{$$ = createRet($3);}
				| RETURN expr ';'			{$$ = createRet($2);}
%%

int yyerror(char *s){
	exit(1);
}

astNode* ret_root(){
	return root;
}