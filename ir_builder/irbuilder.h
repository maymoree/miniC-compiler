#include <llvm-c/Core.h>
#include <ast.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>

using namespace std;

void ir_builder(astNode* root);
void pre_traverse(astNode* node, vector<vector<char*>*>* stack_variable_names);