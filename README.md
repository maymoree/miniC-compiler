# miniC-compiler
### Compiler for miniC files by Memo Tangtipongkul -- April 14th, 2024
### Last edited -- May 31st, 2024

## Compiling and Testing
- run `make` to compile files in all folders
- run `make test` to test `miniC_compiler` file
- run `make valgrind` to test `miniC_compiler` file for mem leaks and errors
- run `make llvm_file` to generate unoptimized LLVM IR into `./assembly_gen/assembly_test.s`

## Test Output
Currently... printed into the terminal and into certain files
### Part 1 and 2 output --> calls ./$(cmplr).out ./assembly_gen/assembly_test.c
- part 1: prints out the ast and runs semantic analysis
- part 2: creates `pt2.ll` file containing module
### Part 3 output --> uses module created from part 2
- part 3: creates `pt3.ll` file 
### Part 4 output --> uses optimized module from part 3
- part 4: creates `pt4.ll` file

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

## Part 4 -- Assembly Code Generation (assembly_gen folder)

**FILES**
1. Register allocation and code generation
   - generator.c
   - generator.h
2. Assembly test
   - assembly_test.c --> in miniC
   - assembly_test.c --> unoptimized, clang LLVM IR


## To Dos:
- [] add info to .h files
- [] make .c files more organized
- [] write test files in bash??