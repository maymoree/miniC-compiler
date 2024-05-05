#include "optimization.h"
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm> 
#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>
#include <llvm-c/Types.h>

using namespace std;

int main(int argc, char** argv)
{
	LLVMModuleRef m;

	if (argc == 2){
		main_optimization(argv[1]);
	}
	else{
		return 1;
	}

	return 0;
}