filename = part1

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

(filename).out: $(filename).l
	yacc -d -t -v $(filename).y 
	lex $(filename).l
	g++ -g lex.yy.c y.tab.c ./ast/ast.c ./ast/smta.c -o $(filename).out

testbad: $(filename).out
	$(VALGRIND) ./$(filename).out ./miniCfiles/semantic_analysis_tests/p4_bad.c

testgood: $(filename).out
	$(VALGRIND) ./$(filename).out ./miniCfiles/p3.c

clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -rf $(filename).out lex.yy.c y.tab.c y.tab.h y.output
	