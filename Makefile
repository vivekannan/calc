all : calc

calc : main.c calc.c helpers.c calc.h helpers.h struct.h
	gcc -std=c99 -o calc main.c calc.c helpers.c -lm 

clean :
	-rm calc
