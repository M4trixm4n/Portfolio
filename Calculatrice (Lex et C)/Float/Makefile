calculette.exe: pile.o lex.o cal.o
	gcc pile.o lex.o cal.o -o calculette
	
pile.o: pile.c
	gcc -c pile.c -o pile.o

cal.o: cal.c
	gcc -c cal.c -o cal.o

lex.yy.c: calculette.lex
	flex calculette.lex

lex.o: lex.yy.c
	gcc -c lex.yy.c -o lex.o