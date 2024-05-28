lex = ./ir_builder/grammar_shared/minic.l
yacc = ./ir_builder/grammar_shared/minic.y
smta = ./frontend/smta
ir = ./ir_builder/irbuilder
opt = ./optimization/optimization
ast = ./ast/ast
cmplr = miniC_compiler

TEST = ./ir_builder/builder_tests/p6

INC = ./ast/ast.c ./frontend/smta.c

CFLAGS = -g -I ./ast -I ./frontend -I ./optimization -I ./ir_builder -I /usr/include/llvm-c-15/

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

$(cmplr).out: $(cmplr).c $(opt).o $(smta).o $(ir).o $(ast).o lex.yy.c y.tab.c
	g++ $(CFLAGS) -o $@ $(cmplr).c lex.yy.c y.tab.c $(opt).o $(ir).o $(smta).o $(ast).o `llvm-config-15 --cxxflags --ldflags --libs core`

$(ir).o: $(ir).c $(ir).h
	g++ $(CFLAGS) -c $< -o $@

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
	./$(cmplr).out ./ir_builder/builder_tests/p2.c ./optimization/optimizer_test_results/p4_const_prop.ll

valgrind: $(cmplr).out
	$(VALGRIND) ./$(cmplr).out ./ir_builder/builder_tests/p2.c ./optimization/optimizer_test_results/p4_const_prop.ll

llvm_file: $(TEST).c
	clang -S -emit-llvm $(TEST).c -o $(TEST).s

clean :
	rm -f *.o y.tab.c y.tab.h y.output lex.yy.c $(cmplr).out 
	rm -f ./optimization/optimization.o ./ast/ast.o ./frontend/smta.o
	rm -f before llvm-ir.s llvm-ir.s-faster peda* .gdb_history
	rm -f pt2.ll pt3.ll