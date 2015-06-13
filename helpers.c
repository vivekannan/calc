#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define G 1.6180339887498948482
#define E 2.7182818284590452354
#define PI 3.14159265358979323846

#define OP_COUNT 10
#define FUNC_COUNT 21

char* operators = "+-*/%^$~_!";
char* functions[] = { "sin", "cos", "tan", "asin", "acos", "atan", "sinh", "cosh", "tanh", "asinh", "acosh", "atanh", "exp", "floor", "ceil", "round", "log", "ln", "sqrt", "abs", "sgn" };

int isFunction(char* s) {
	
	int i = FUNC_COUNT - 1;
	
	for(; i >= 0 && strcmp(functions[i], s) != 0; i--);
	
	return i;
}

int isOperator(char c) {
	
	int i = OP_COUNT - 1;
	
	for(; i >= 0 && *(operators + i) != c; i--);
	
	return i;
}

double isSymbol(char* s) {
	
	if(strcmp(s, "e") == 0)
		return E;
	
	if(strcmp(s, "pi") == 0)
		return PI;
	
	if(strcmp(s, "inf") == 0)
		return INFINITY;
	
	if(strcmp(s, "rand") == 0) {
		srand((unsigned) time(NULL) + rand());
		
		return (double) rand() / (double) RAND_MAX;
	}
	
	if(strcmp(s, "g") == 0)
		return G;
	
	return 0.0;
}

double factorial(double d) {
	
	double result = 1;
	
	for(; d > 1; d--)
		result *= d;
	
	return result;
}

double toRadians(double d) {
	
	return d * (PI / 180.0);
}

double toDegrees(double d) {
	
	return d * (180.0 / PI);
}
