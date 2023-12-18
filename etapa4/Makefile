etapa4: lex.yy.o parser.tab.o main.o asd.o table.o
	gcc -g -o etapa4 lex.yy.o parser.tab.o main.o asd.o table.o

lex.yy.o parser.tab.o main.o asd.o table.o: lex.yy.c parser.tab.c asd.c table.c main.c
	gcc -g -c lex.yy.c main.c asd.c table.c parser.tab.c

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y --report-file=report-file.txt

clean:
	rm -rf lex.yy.* parser.tab.* main.o asd.o table.o etapa4 *.dot

entrega:
	tar cvzf etapa4.tgz asd.c asd.h table.c table.h main.c Makefile parser.y scanner.l