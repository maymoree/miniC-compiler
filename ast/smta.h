#include "ast.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

int semantic_analysis(astNode* root);
void traverse(vector<vector<char*>*> *stack_symbol_table, astNode* node);