#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <string.h>

#include "struct.h"
#include "helpers.h"

static int USE_DEGREE = 0;
static int USE_NEWLINE = 0;
static char* HELP_TEXT = "This is help.";

static int inCount;
static struct token* inQueue;

static int opCount;
static struct token* opStack;

static int outCount;
static double* outQueue;

double factorial(double d) {
	
	double result = 1;
	
	for(; d > 1; d--)
		result *= d;
	
	return result;
}

double toRadians(double d) {
	return d * (3.14159265 / 180.0);
}

double toDegrees(double d) {
		return d * (180.0 / 3.14159265);
}

void printMessage(char* message, int exitCode) {
	
	printf("%s\n", message);
	exit(exitCode);
}

int parseOptions(int argc, char* argv[]) {
	
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-n") == 0)
			USE_NEWLINE = 1;
		
		else if(strcmp(argv[i], "-d") == 0)
			USE_DEGREE = 1;
		
		else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
			printMessage(HELP_TEXT, 1);
		
		else
			return i;
	}
	
	printMessage("calc: Expression missing.\nUsage: calc [OPTIONS] EXPRESSION.\nTry 'calc --help' for more information.", -1);
}

void tokenize(char* expr) {
	
	int i;
	char c;
	char* s;
	double d;
	struct token temp;
	
	while((c = *expr++) != '\0') {
		
		if(isspace(c))
			continue;
		
		if(isalpha(c)) {
			i = 0;
			s = (char*) malloc((strlen(expr) + 2) * sizeof(char));
			
			do {
				*(s + i++) = c;
			} while((c = *expr++) != '\0' && isalpha(c));
			
			*(s + i) = '\0';
			
			if(!isFunction(s))
				printMessage("Undefined Function", -3);
			
			temp.type = FUNCTION;
			temp.data.func = s;
			temp.precedence = 3;
			temp.leftAssociative = 0;
			
			inQueue[inCount++] = temp;
			
			--expr;
		}
		
		else if(isdigit(c) || c == '.') {
			d = strtod(--expr, &s);
			
			if(expr == (char*) s)
				printMessage("Invalid constant", -3);
			
			if(errno != 0)
				printMessage("Constant to large for double.", -3);
			
			temp.type = CONSTANT;
			temp.data.d = d;
			
			inQueue[inCount++] = temp;
			
			expr = (char*) s;
		}
		
		else if(c == '(' || c == ')') {
			temp.type = PARENTHESIS;
			temp.data.op = c;
			
			inQueue[inCount++] = temp;
		}
		
		else if(isOperator(c)) {
			
			temp.type = OPERATOR;
			temp.data.op = c;
			temp.precedence = precedence(c);
			temp.leftAssociative = isLeftAssociative(c);
			
			inQueue[inCount++] = temp;
		}
		
		else
			printMessage("Invalid token.", -3);
	}
}

void execute(struct token temp) {
	
	double d1, d2, result;
	
	if(temp.type == OPERATOR) {
		if(isBinary(temp.data.op) == 1) {
			if(outCount < 2)
				printMessage("Malformed Expression.", -5);
			
			d2 = outQueue[--outCount];
			d1 = outQueue[--outCount];
			
			switch(temp.data.op) {
				case '+':
					result = d1 + d2;
					break;
				case '-':
					result = d1 - d2;
					break;
				case '*':
					result = d1 * d2;
					break;
				case '/':
					if(d2 == 0.0)
						printMessage("Division by Zero.", -5);
					
					result = d1 / d2;
					break;
				case '%':
					if(d2 == 0.0)
						printMessage("Modulo by Zero.", -5);
					
					result = fmod(d1, d2);
					break;
				case '^':
					if(d1 == 0.0 && d2 == 0.0)
						printMessage("The power of zero is undefined for zero exponent.", -5);
					else if(d1 < 0.0 && floor(d2) != d2)
						printMessage("The power of negative base for decimal exponent is not supported.", -5);
					else if(d1 == 0.0 && d2 < 0.0)
						printMessage("The power of zero is undefined for a negative exponent.", -5);
					else
						result = pow(d1, d2);
					break;
			}
		}
		
		else {
			if(outCount < 1)
				printMessage("Malformed Expression.", -5);
			
			d1 = outQueue[--outCount];
			
			switch(temp.data.op) {
				case '_':
					result = d1;
					break;
				case '~':
					result = -d1;
					break;
				case '!':
					if(floor(d1) != d1 || d1 < 0.0)
						printMessage("Factorial is only defined for natural numbers.", -5);
					
					result = factorial(d1);
					break;
			}
		}
	}
	
	else {
		if(outCount < 1)
			printMessage("Malformed Expression.", -5);
		
		d1 = outQueue[--outCount];
		
		if(strcmp(temp.data.func, "sin") == 0)
			result = USE_DEGREE ? sin(toRadians(d1)) : sin(d1);
		
		else if(strcmp(temp.data.func, "cos") == 0)
			result = USE_DEGREE ? cos(toRadians(d1)) : cos(d1);
		
		else if(strcmp(temp.data.func, "tan") == 0)
			result = USE_DEGREE ? tan(toRadians(d1)) : tan(d1);
		
		else if(strcmp(temp.data.func, "sinh") == 0)
			result = USE_DEGREE ? sinh(toRadians(d1)) : sinh(d1);
		
		else if(strcmp(temp.data.func, "cosh") == 0)
			result = USE_DEGREE ? cosh(toRadians(d1)) : cosh(d1);
		
		else if(strcmp(temp.data.func, "tanh") == 0)
			result = USE_DEGREE ? tanh(toRadians(d1)) : tanh(d1);
		
		else if(strcmp(temp.data.func, "asin") == 0)
			result = USE_DEGREE ? toDegrees(asin(d1)) : asin(d1);
		
		else if(strcmp(temp.data.func, "acos") == 0)
			result = USE_DEGREE ? toDegrees(acos(d1)) : acos(d1);
		
		else if(strcmp(temp.data.func, "atan") == 0)
			result = USE_DEGREE ? toDegrees(atan(d1)) : atan(d1);
		
		else if(strcmp(temp.data.func, "asinh") == 0)
			result = USE_DEGREE ? toDegrees(asinh(d1)) : asinh(d1);
		
		else if(strcmp(temp.data.func, "acosh") == 0)
			result = USE_DEGREE ? toDegrees(acosh(d1)) : acosh(d1);
		
		else if(strcmp(temp.data.func, "atanh") == 0)
			result = USE_DEGREE ? toDegrees(atanh(d1)) : atanh(d1);
		
		else if(strcmp(temp.data.func, "log") == 0) {
			if(d1 <= 0.0)
				printMessage("Logarithm is defined only for numbers > 0.", -5);
			result = log10(d1);
		}
		
		else if(strcmp(temp.data.func, "ln") == 0) {
			if(d1 <= 0.0)
				printMessage("Logarithm is defined only for numbers > 0.", -5);
			result = log(d1);
		}
		
		else if(strcmp(temp.data.func, "exp") == 0)
			result = exp(d1);
		
		else if(strcmp(temp.data.func, "round") == 0)
			result = round(d1);
		
		else if(strcmp(temp.data.func, "ceil") == 0)
			result = ceil(d1);
		
		else if(strcmp(temp.data.func, "floor") == 0)
			result = floor(d1);
		
		else if(strcmp(temp.data.func, "sqrt") == 0) {
			if(d1 < 0.0)
				printMessage("Square for negative numbers is not supported.", -5);
			result = sqrt(d1);
		}
		
		else if(strcmp(temp.data.func, "abs") == 0)
			result = abs(d1);
	}
	
	outQueue[outCount++] = result;
}

void shuntYard() {
	
	int unary = 1;
	struct token temp;
	
	for(int i = 0; i < inCount; i++) {
		temp = inQueue[i];
		
		if(temp.type == CONSTANT)
			outQueue[outCount++] = temp.data.d;
		
		else if(temp.type == PARENTHESIS) {
			if(temp.data.op == '(')
				opStack[opCount++] = temp;
			
			else if(temp.data.op == ')') {
				
				while(1) {
					if(opCount == 0)
						printMessage("Mismatched ')'.", -2);
					
					temp = opStack[--opCount];
					
					if(temp.data.op == '(')
						break;
					
					if(temp.type == CONSTANT)
						outQueue[outCount++] = temp.data.d;
					else
						execute(temp);
				}
				temp.data.op = ')';
			}
		}
		
		else if(temp.type == OPERATOR) {
			if(unary == 1) {
				if(temp.data.op == '-')
					temp.data.op = '~';
				
				else if(temp.data.op == '+')
					temp.data.op = '_';
				
				temp.precedence = precedence(temp.data.op);
				temp.leftAssociative = isLeftAssociative(temp.data.op);
			}
			
			if(temp.leftAssociative)
				while(opCount != 0 && (opStack[opCount - 1].type == OPERATOR || opStack[opCount - 1].type == FUNCTION) && temp.precedence <= opStack[opCount - 1].precedence)
					execute(opStack[--opCount]);
			
			else
				while(opCount != 0 && (opStack[opCount - 1].type == OPERATOR || opStack[opCount - 1].type == FUNCTION) && temp.precedence < opStack[opCount - 1].precedence)
					execute(opStack[--opCount]);
			
			opStack[opCount++] = temp;
		}
		
		else {
			opStack[opCount++] = temp;
		}
		
		unary = !(temp.type == CONSTANT || (temp.type == PARENTHESIS && temp.data.op == ')'));
	}
	
	while(opCount != 0) {
		if(opStack[opCount - 1].data.op == '(')
			printMessage("Mismatched '('.", -2);
		
		execute(opStack[--opCount]);
	}
	
	if(outCount != 1)
		printMessage("Malformed expression.", -6);
	
	printf("%g%c", outQueue[--outCount], USE_NEWLINE ? '\n' : ' ');
}

void evaluate(char* expr) {
	
	inCount = 0;
	inQueue = (struct token*) malloc(strlen(expr) * sizeof(struct token));
	
	opCount = 0;
	opStack = (struct token*) malloc(strlen(expr) * sizeof(struct token));
	
	outCount = 0;
	outQueue = (double*) malloc(strlen(expr) * sizeof(double));
	
	tokenize(expr);
	shuntYard();
	
	free(inQueue);
	free(opStack);
	free(outQueue);
}