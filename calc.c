#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <string.h>

#include "struct.h"
#include "helpers.h"

#define HELP_TEXT "Usage: calc [OPTION] <EXPRESSION>...\n\
\n\
Options\n\
\n\
  The following flags can be used,\n\
\n\
  1. -d, consider all angles to be degrees. Results are also in degrees.\n\
  2. -n, print results of each epressions in a new line instead of separating them by space.\n\
  3. -h (or) --help, print help and exit.\n\
\n\
Visit https://github.com/vivekannan/calc/ for more info.\n\
"

static int USE_DEGREE = 0;
static int USE_NEWLINE = 0;

static int resultCount;
static double* results;

static int opCount;
static struct token* opStack;

static int outCount;
static double* outStack;

static const int binary[] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
static const int precedence[] = { 0, 0, 1, 1, 1, 2, 3, 4, 4, 5 };
static const int association[] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };

enum { SIN, COS, TAN, ASIN, ACOS, ATAN, SINH, COSH, TANH, ASINH, ACOSH, ATANH, EXP, FLOOR, CEIL, ROUND, LOG, LN, SQRT, ABS, SGN };

int parseOptions(int argc, char* argv[]) {
	
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-n") == 0)
			USE_NEWLINE = 1;
		
		else if(strcmp(argv[i], "-d") == 0)
			USE_DEGREE = 1;
		
		else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf("%s", HELP_TEXT);
			exit(1);
		}
		
		else
			return i;
	}
	
	fprintf(stderr, "calc: Expression missing.\nUsage: calc [OPTIONS] EXPRESSIONS.\nTry 'calc --help' for more information.\n");
	exit(-1);
}

int execute(struct token temp) {
	
	double d1, d2, result;
	
	if(temp.type == OPERATOR) {
		if(temp.isBinary) {
			if(outCount < 2) {
				fprintf(stderr, "Malformed Expression.");
				return 0;
			}
			
			d2 = outStack[--outCount];
			d1 = outStack[--outCount];
			
			switch(temp.subType) {
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
					result = d1 / d2;
					break;
				case '%':
					result = fmod(d1, d2);
					break;
				case '^':
					result = pow(d1, d2);
					break;
			}
		}
		
		else {
			if(outCount < 1) {
				fprintf(stderr, "Malformed Expression.");
				return 0;
			}
			
			d1 = outStack[--outCount];
			
			switch(temp.subType) {
				case '_':
					result = d1;
					break;
				case '~':
					result = -d1;
					break;
				case '!':
					if(floor(d1) != d1 || d1 < 0.0 || d1 == INFINITY) {
						fprintf(stderr, "Factorial is only defined for natural numbers.");
						return 0;
					}
					
					result = factorial(d1);
					break;
				case '$':
					if(floor(d1) != d1 || d1 <= 0 || d1 > resultCount) {
						fprintf(stderr, "Invalid result index.");
						return 0;
					}
					
					result = results[(int) d1 - 1];
					break;
			}
		}
	}
	
	else {
		if(outCount < 1) {
			fprintf(stderr, "Malformed Expression.");
			return 0;
		}
		
		d1 = outStack[--outCount];
		
		if(temp.subType == SIN)
			result = USE_DEGREE ? sin(toRadians(d1)) : sin(d1);
		
		else if(temp.subType == COS)
			result = USE_DEGREE ? cos(toRadians(d1)) : cos(d1);
		
		else if(temp.subType == TAN)
			result = USE_DEGREE ? tan(toRadians(d1)) : tan(d1);
		
		else if(temp.subType == SINH)
			result = USE_DEGREE ? sinh(toRadians(d1)) : sinh(d1);
		
		else if(temp.subType == COSH)
			result = USE_DEGREE ? cosh(toRadians(d1)) : cosh(d1);
		
		else if(temp.subType == TANH)
			result = USE_DEGREE ? tanh(toRadians(d1)) : tanh(d1);
		
		else if(temp.subType == ASIN)
			result = USE_DEGREE ? toDegrees(asin(d1)) : asin(d1);
		
		else if(temp.subType == ACOS)
			result = USE_DEGREE ? toDegrees(acos(d1)) : acos(d1);
		
		else if(temp.subType == ATAN)
			result = USE_DEGREE ? toDegrees(atan(d1)) : atan(d1);
		
		else if(temp.subType == ASINH)
			result = USE_DEGREE ? toDegrees(asinh(d1)) : asinh(d1);
		
		else if(temp.subType == ACOSH)
			result = USE_DEGREE ? toDegrees(acosh(d1)) : acosh(d1);
		
		else if(temp.subType == ATANH)
			result = USE_DEGREE ? toDegrees(atanh(d1)) : atanh(d1);
		
		else if(temp.subType == LOG)
			result = log10(d1);
		
		else if(temp.subType == LN)
			result = log(d1);
		
		else if(temp.subType == EXP)
			result = exp(d1);
		
		else if(temp.subType == ROUND)
			result = round(d1);
		
		else if(temp.subType == CEIL)
			result = ceil(d1);
		
		else if(temp.subType == FLOOR)
			result = floor(d1);
		
		else if(temp.subType == SQRT)
			result = sqrt(d1);
		
		else if(temp.subType == ABS)
			result = fabs(d1);
		
		else if(temp.subType == SGN)
			result = d1 > 0 ? 1 : (d1 < 0 ? -1 : 0);
	}
	
	outStack[outCount++] = result;
	
	return 1;
}

int emptyOpStack() {
	
	while(opCount != 0) {
		if(opStack[opCount - 1].type == PARENTHESIS) {
			fprintf(stderr, "Mismatched '('.");
			return 0;
		}
		
		if(!execute(opStack[--opCount]))
			return 0;
	}
	
	if(outCount != 1) {
		fprintf(stderr, "Malformed expression.");
		return 0;
	}
	
	*(results + resultCount++) = outStack[--outCount];
	
	return 1;
}

int shuntYard(char* expr) {
	
	int i;
	char c;
	char* s;
	double d;
	int unary = 1;
	struct token temp;
	
	while((c = tolower(*expr++)) != '\0') {
		
		if(isspace(c))
			continue;
		
		if(isalpha(c)) {
			i = 0;
			s = (char*) malloc((strlen(expr) + 2) * sizeof(char));
			
			if(!s) {
				fprintf(stderr, "Cannot allocate memory.");
				exit(-2);
			}
			
			do {
				*(s + i++) = c;
			} while(isalpha(c = tolower(*expr++)));
			
			*(s + i) = '\0';
			
			if((i = isFunction(s)) != -1) {
				temp.subType = i;
				temp.precedence = 3;
				temp.type = FUNCTION;
				temp.leftAssociative = 0;
				
				opStack[opCount++] = temp;
				unary = 1;
			}
			
			else if((d = isSymbol(s))) {
				outStack[outCount++] = d;
				unary = 0;
			}
			
			else {
				fprintf(stderr, "Undefined function/symbol \"%s\".", s);
				return 0;
			}
			
			--expr;
			free(s);
		}
		
		else if(isdigit(c) || c == '.') {
			errno = 0;
			d = strtod(--expr, &s);
			
			if(expr == (char*) s) {
				fprintf(stderr, "Invalid constant.");
				return 0;
			}
			
			if(errno != 0) {
				fprintf(stderr, "Constant to large for double.");
				return 0;
			}
			
			outStack[outCount++] = d;
			unary = 0;
			expr = (char*) s;
		}
		
		else if(c == '(') {
			temp.type = PARENTHESIS;
			
			opStack[opCount++] = temp;
			unary = 1;
		}
		
		else if(c == ')') {
			while(1) {
				if(opCount == 0) {
					fprintf(stderr, "Mismatched ')'.");
					return 0;
				}
				
				temp = opStack[--opCount];
				
				if(temp.type == PARENTHESIS)
					break;
				
				if(!execute(temp))
					return 0;
			}
			unary = 0;
		}
		
		else if(c == ',') {
			if(!emptyOpStack())
				return 0;
			unary = 1;
		}
		
		else if((i = isOperator(c)) != -1) {
			if(unary == 1) {
				if(c == '-') {
					c = '~';
					i = 7;
				}
				
				else if(c == '+') {
					c = '_';
					i = 8;
				}
			}
			
			temp.subType = c;
			temp.type = OPERATOR;
			temp.isBinary = binary[i];
			temp.precedence = precedence[i];
			temp.leftAssociative = association[i];
			
			while(opCount != 0 && opStack[opCount - 1].type != PARENTHESIS && temp.precedence < (opStack[opCount - 1].precedence + temp.leftAssociative))
				if(!execute(opStack[--opCount]))
					return 0;
			
			unary = 1;
			opStack[opCount++] = temp;
		}
		
		else {
			fprintf(stderr, "Invalid token. %c", c);
			return 0;
		}
	}
	
	if(!emptyOpStack())
		return 0;
	
	for(int i = 0; i < resultCount; i++)
		printf("%.10G%s", *(results + i), resultCount - i == 1 ? "" : ",");
	
	return 1;
}

void evaluate(char* expr, int addEndChar) {
	
	opCount = 0;
	opStack = (struct token*) malloc(strlen(expr) * sizeof(struct token));
	
	outCount = 0;
	outStack = (double*) malloc(strlen(expr) * sizeof(double));
	
	resultCount = 0;
	results = (double*) malloc(strlen(expr) * sizeof(double));
	
	if(!opStack || !outStack || !results) {
		fprintf(stderr, "Cannot allocate memory.");
		exit(-2);
	}
	
	if(shuntYard(expr))
		printf("%s", addEndChar ? (USE_NEWLINE ? "\n" : ";") : "");
	
	else
		fprintf(stderr, "%s", addEndChar ? (USE_NEWLINE ? "\n" : ";") : "");
	
	free(opStack);
	free(outStack);
	free(results);
}

int main(int argc, char* argv[]) {
	
	setbuf(stdout, NULL);
	
	int i = parseOptions(argc, argv);
	
	for(; i < argc; i++)
		evaluate(argv[i], argc - i != 1);
	
	printf("\n");
	
	return 0;
}