filename = part1
$(filename).out: $(filename).l
	lex $(filename).l
	gcc lex.yy.c -o $(filename).out

clean:
	rm -rf $(filename).out lex.yy.c
