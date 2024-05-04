filename = frontend_test

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

(filename): $(filename).c
	yacc -d -t -v part1.y -Wcounterexamples
	lex part1.l 
	g++ -g -o $(filename) $(filename).c lex.yy.c y.tab.c ../ast/ast.c ./smta.c -o $(filename)

smta_bad: $(filename)
	$(VALGRIND) ./$(filename) ../miniCfiles/semantic_analysis_tests/p4_bad.c

smta_good: $(filename)
	$(VALGRIND) ./$(filename) ../miniCfiles/semantic_analysis_tests/p1_good.c

clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -rf $(filename) lex.yy.c y.tab.c y.tab.h frontend_test

