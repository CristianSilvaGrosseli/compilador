all: etapa2

parser.tab.c: parser.y
	@(bison -d parser.y)

lex.yy.c: scanner.l
	@(flex scanner.l)

etapa2: parser.tab.c lex.yy.c main.c helpers.h
	@(gcc -o etapa2 main.c parser.tab.c lex.yy.c)

clean:
	rm lex.yy.c parser.tab.c parser.tab.h etapa2