# miniC-compiler
### Compiler for miniC files by Memo Tangtipongkul -- April 14, 2024

## Part 1 -- Compiler (lex and yacc files)
- run 'make' to compile all files
- run 'make testbad' to see outcome of failed semantic analysis tests
- run 'make testgood' to see outcome of passed semantic analysis tests

- AST and parsing works without any errors or mem leaks
    - part1.l part1.y 
- Semantic Analysis works
    - smta.c smta.h
- Note: All shift/reduce conflicts resolved!!!

## Part 3 -- Optimization (optimization folder)
- run 'make' to compile optimization file
- run 'make llvm_file' to get .ll files for testing

- optimization.c
- optimization.h
- Note: have common sub expr function done

#### organize the repo/files better AND add info to .h files!