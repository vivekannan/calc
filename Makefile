all : calc

calc : main.c calc.c helpers.c
	gcc -std=c99 -o calc main.c calc.c helpers.c -lm 

clean :
	-rm calc *.o
