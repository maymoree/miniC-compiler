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
- Note: Code has 1 error and 42 bytes of mem leaks in certain cases; shift/reduce conflict has yet to be resolved
