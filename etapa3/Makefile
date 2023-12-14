etapa3: lex.yy.o parser.tab.o main.o asd.o
	gcc -g -o etapa3 lex.yy.o parser.tab.o main.o asd.o

lex.yy.o parser.tab.o main.o asd.o: lex.yy.c parser.tab.c asd.c main.c
	gcc -g -c lex.yy.c main.c asd.c parser.tab.c

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y --report-file=report-file.txt

clean:
	rm -rf lex.yy.* parser.tab.* main.o asd.o etapa3 *.dot

entrega:
	tar cvzf etapa3.tgz asd.c asd.h main.c Makefile parser.y scanner.l