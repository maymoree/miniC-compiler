%{
#include "../ast/ast.h"
#include "y.tab.h"
#include <stdio.h>
#include <string.h>
extern int yylex();

%}


%%
print       {return PRINT;}               //functions
read        {return READ;}
return      {return RETURN;}
if          {return IF;}                 //conditions
else        {return ELSE;}
while       {return WHILE;}
int         {return INT;}                     //types
void		{return VOID;}
extern		{return EXTERN;}
[+-]?[0-9]+         {                                                       //integers
                        yylval.ival = atoi(yytext);
						return NUM;
                    }
[a-zA-Z][a-zA-Z0-9]*    {                                                   //variable names
                        yylval.idname = strdup(yytext);
						return NAME; // free in yacc file
                        }
[-=+*/><(){};]      {return yytext[0];}        //operators
">="          {return GE;}                     // greater equal to
"<="          {return LE;}                     // lesser equal to
"=="          {return EQ;}                     // equal to
"!="          {return NEQ;}                     // not equal to
\n            {yylineno++;}                                   
[ \t]                                                            
.                                                                 
%%

int yywrap(){
    return 1;
}