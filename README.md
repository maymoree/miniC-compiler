# miniC-compiler
### Compiler for miniC files by Memo Tangtipongkul -- April 14, 2024

#### Have to go into each folder to compile/run make

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
3. Front end Test
   - frontend_test.c

## Part 3 -- Optimization (optimization folder)
- run `make` to compile optimization file
- run `make llvm_file` to get .ll files for testing

**FILES**
1. Optimization
   - optimization.c
      - module for optimization
   - optimization_test.c
      - can individually test optimization: `make test` and `make valgrind`
   - optimization.h

## To Dos:
- [] restructure code to 1 main.c
- [] add info to .h files
- [x] global optimization
- [x] check error in local optimization -- had to check operand was constant
- [x] constant folding
- [x] thoroughly test local optimization
- [x] check valgrind for local optimization
