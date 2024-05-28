# miniC-compiler
### Compiler for miniC files by Memo Tangtipongkul -- April 14, 2024

## Compiling
- run `make` to compile files in all folders
- run `make test` to test `miniC_compiler` file
- run `make valgrind` to test `miniC_compiler` file for mem leaks and errors

## Test Output
Currently... have not created bash test files a nicely formatted output
### Part 1 and 2 output --> calls ./ir_builder/builder_test/p2.c
- part 1: prints out the ast and runs semantic analysis
- part 2: creates `pt2.ll` file containing module
### Part 3 output --> uses module created from part 2
- creates `pt3.ll` file 

## Part 1 -- Frontend (frontend folder)

**FILES**
1. Parsing and Creating AST
   - part1.l
   - part1.y 
2. Semantic Analysis
   - smta.c
   - smta.h


## Part 2 -- IR Builder  (ir_builder folder)

**FILES**
1. Preprocessing and building
   - irbuilder.c
   - irbuilder.h
2. Pseudocode
   - pseudocode.txt

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
