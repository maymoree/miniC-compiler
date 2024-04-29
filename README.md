# miniC-compiler
### Compiler for miniC files by Memo Tangtipongkul -- April 14, 2024

## Part 1 -- Frontend (frontend folder)
- run `make` to compile all files
- run `make smta_bad` to see outcome of **failed semantic analysis** tests
- run `make smta_good` to see outcome of **passed semantic analysis** tests

**FILES**
1. Parsing and Creating AST
   - part1.l
   - part1.y 
2. Semantic Analysis
   - smta.c
   - smta.h

## Part 3 -- Optimization (optimization folder)
- run `make` to compile optimization file
- run `make llvm_file` to get .ll files for testing

**FILES**
1. Optimization
   - optimization.c
   - optimization.h

## To Dos:
- [] add info to .h files
- [] global optimization
- [] check error in local optimization -- something with invalid write
- [x] constant folding
- [x] thoroughly test local optimization
- [x] check valgrind for local optimization
