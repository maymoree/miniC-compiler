# miniC-compiler
### Compiler for miniC files by Memo Tangtipongkul -- April 14, 2024

## Compiling
- run `make` to compile files in all folders
- run `make test` to test `miniC_compiler` file
- run `make valgrind` to test `miniC_compiler` file for mem leaks and errors

## Test Output
Currently... have not created bash test files a nicely formatted output
### Part 1 output --> calls ./miniCfiles/p1.c
- prints out the ast
### Part 3 output --> calls ./optimization/optimizer_test_results/p4_const_prop.ll
- prints out the optimized instructions
   - each block is marked by ***List of Instructions-------------***


## Part 1 -- Frontend (frontend folder)

**FILES**
1. Parsing and Creating AST
   - part1.l
   - part1.y 
2. Semantic Analysis
   - smta.c
   - smta.h

## Part 3 -- Optimization (optimization folder)

**FILES**
1. Optimization
   - optimization.c
      - module for optimization
   - optimization.h

## To Dos:
- [] add info to .h files
- [] write test files in bash??
- [x] restructure code to 1 main.c
