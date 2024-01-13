etapa5: lex.yy.o parser.tab.o main.o asd.o iloc.o
	gcc -g -o etapa5 lex.yy.o parser.tab.o main.o asd.o

lex.yy.o parser.tab.o main.o asd.o iloc.o: lex.yy.c parser.tab.c asd.c iloc.c main.c 
	gcc -g -c lex.yy.c main.c asd.c iloc.c parser.tab.c

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y -Wcounterexamples --report-file=report-file.txt

clean:
	rm -rf lex.yy.* parser.tab.* main.o asd.o iloc.o etapa5 *.dot

entrega:
	tar cvzf etapa5.tgz asd.c asd.h iloc.c iloc.h main.c Makefile parser.y scanner.l