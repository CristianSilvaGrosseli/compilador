etapa7: lex.yy.o parser.tab.o main.o tad_ast.o table.o iloc.o
	gcc -g -o etapa7 lex.yy.o parser.tab.o main.o tad_ast.o table.o iloc.o

lex.yy.o parser.tab.o main.o tad_ast.o table.o iloc.o: lex.yy.c parser.tab.c tad_ast.c table.c iloc.c main.c
	gcc -g -c lex.yy.c main.c tad_ast.c table.c iloc.c parser.tab.c

lex.yy.c: scanner.l
	flex scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y -Wcounterexamples --report-file=report-file.txt

clean:
	rm -rf lex.yy.* parser.tab.* main.o tad_ast.o table.o iloc.o etapa7 *.dot

entrega:
	tar cvzf etapa7.tgz helpers.h tad_ast.c tad_ast.h table.c table.h iloc.c iloc.h main.c encontra_lider.c Makefile parser.y scanner.l