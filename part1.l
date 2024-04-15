%{
#include <stdio.h>
%}


%%
print       {printf("function: %s\n", yytext);}               //functions; are orders correct??!!!
read        {printf("function: %s\n", yytext);}
return      {printf("function: %s\n", yytext);}
if          {printf("condition: %s\n", yytext);}                 //conditions
else        {printf("condition: %s\n", yytext);}
while       {printf("loop: %s\n", yytext);}
int         {printf("type: %s\n", yytext);}                     //types
void		{printf("type: %s\n", yytext);}
extern		{printf("type: %s\n", yytext);}
[+-]?[0-9]+         {                                                       //integers
                        printf("num: %s\n", yytext);
                        // yylval.ival = atoi(yytext);
                    }
[a-zA-Z][a-zA-Z0-9]*    {                                                   //variable names
                        printf("var: %s\n", yytext);
                        // yylval.sname = strdup(yytext);
                        }
[-=+*/><(){};]      {printf("opr: %s\n", yytext);}        //operators
">="          {printf("opr: %s\n", yytext);}                     // greater equal to
"<="          {printf("opr: %s\n", yytext);}                     // lesser equal to
"=="          {printf("opr: %s\n", yytext);}                     // equal to
"!="          {printf("opr: %s\n", yytext);}                     // not equal to
\n                                                               
[ \t]                                                            
.                                                                 
%%


int yywrap(){
    return 0;
}

int main(int argc, char* argv[]){
	if (argc == 2){
		yyin = fopen(argv[1], "r");
		if (yyin == NULL){
			printf("Error\n");
			return(1);
		}
	}
	
	yylex();
	return(0);
}
