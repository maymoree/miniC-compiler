filename = part1
(filename).out: $(filename).l
	yacc -d -t -v $(filename).y
	lex $(filename).l
	g++ -g lex.yy.c y.tab.c ./ast/ast.c -o $(filename).out

clean:
	rm -rf $(filename).out lex.yy.c y.tab.c y.tab.h y.output