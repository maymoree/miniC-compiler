%{
#include "./ast/ast.h"
#include "y.tab.h"
#include <stdio.h>
#include <string.h>
extern int yylex();
%}


%%
print       {printf("function: %s\n", yytext); return PRINT;}               //functions
read        {printf("function: %s\n", yytext); return READ;}
return      {printf("function: %s\n", yytext); return RETURN;}
if          {printf("condition: %s\n", yytext); return IF;}                 //conditions
else        {printf("condition: %s\n", yytext); return ELSE;}
while       {printf("loop: %s\n", yytext); return WHILE;}
int         {printf("type: %s\n", yytext); return INT;}                     //types
void		{printf("type: %s\n", yytext); return VOID;}
extern		{printf("type: %s\n", yytext); return EXTERN;}
[+-]?[0-9]+         {                                                       //integers
                        printf("num: %s\n", yytext);
                        yylval.ival = atoi(yytext);
						return NUM;
                    }
[a-zA-Z][a-zA-Z0-9]*    {                                                   //variable names
                        printf("name: %s\n", yytext);
                        yylval.sname = strdup(yytext);
						return NAME;
                        }
[-=+*/><(){};]      {printf("opr: %s\n", yytext); return yytext[0];}        //operators
">="          {printf("opr: %s\n", yytext); return GE;}                     // greater equal to
"<="          {printf("opr: %s\n", yytext); return LE;}                     // lesser equal to
"=="          {printf("opr: %s\n", yytext); return EQ;}                     // equal to
"!="          {printf("opr: %s\n", yytext); return NEQ;}                     // not equal to
\n                                                               
[ \t]                                                            
.                                                                 
%%

int yywrap(){
    return 1;
}