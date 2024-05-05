lex = ./frontend/part1.l
yacc = ./frontend/part1.y
smta = ./frontend/smta
opt = ./optimization/optimization
ast = ./ast/ast
cmplr = miniC_compiler

INC = ./ast/ast.c ./frontend/smta.c

CFLAGS = -g -I ./ast -I ./frontend -I ./optimization -I /usr/include/llvm-c-15/

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

$(cmplr).out: $(cmplr).c $(opt).o $(smta).o $(ast).o lex.yy.c y.tab.c
	g++ $(CFLAGS) -o $@ $(cmplr).c lex.yy.c y.tab.c $(opt).o $(smta).o $(ast).o `llvm-config-15 --cxxflags --ldflags --libs core`

$(ast).o: $(ast).c $(ast).h
	g++ $(CFLAGS) -c $< -o $@

$(opt).o: $(opt).c $(opt).h
	g++ $(CFLAGS) -c $< -o $@

$(smta).o: $(smta).c $(smta).h
	g++ $(CFLAGS) -c $< -o $@

lex.yy.c : $(lex) y.tab.h
	lex $<

y.tab.h y.tab.c : $(yacc)
	yacc -d -v $<		

test: $(cmplr).out
	./$(cmplr).out ./miniCfiles/p1.c ./optimization/optimizer_test_results/p4_const_prop.ll

valgrind: $(cmplr).out
	$(VALGRIND) ./$(cmplr).out ./miniCfiles/p1.c ./optimization/optimizer_test_results/p4_const_prop.ll

clean :
	rm -f *.o y.tab.c y.tab.h y.output lex.yy.c $(cmplr).out 
	rm -f ./optimization/optimization.o ./ast/ast.o ./frontend/smta.o
	rm -f before llvm-ir.s llvm-ir.s-faster peda* .gdb_history