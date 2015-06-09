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
Operators\n\
\n\
The following operators are supported,\n\
\n\
    + --> addition\n\
    - --> subtraction\n\
    * --> multiplication\n\
    / --> division\n\
    % --> modulo\n\
    ! --> factorial\n\
    ^ --> exponent\n\
\n\
Unary '+' and '-' are also supported. '~' and '_' operators act as aliases for unary '-' and '+' respectively.\n\
\n\
The '$' (dollar) operator can be used to refer to the result of a particular sub-expression. Sub-expression are created using the ',' (comma) operator. For example,\n\
\n\
    ./calc '7 - 5, sin$1, log($1 - -$2)'\n\
\n\
The expression has three sub-expression.\n\
\n\
    $1 = 7 - 5\n\
    $2 = sin$1 = sin(7 - 5)\n\
    $3 = log($1 - -$2) = log(7 - 5 - -sin(7 - 5))\n\
\n\
The results of all three expressions is printed in comma separated format.\n\
\n\
    2,0.9092974268,0.4637881228\n\
\n\
Functions\n\
\n\
  The following function are supported,\n\
  \n\
    sin, cos, tan, asin, acos, atan, sinh, cosh, tanh, asinh, acosh, atanh, log (base 10), ln (base e), exp (e raised), floor, ceil, round, sqrt, abs.\n\
  \n\
  Trignometric function may have infinite precision related issues. For example,\n\
  \n\
    ./calc 'sin pi' 'cos pi'\n\
  \n\
  Results in,\n\
  \n\
    7.338823075E-05 -0.9999999973\n\
  \n\
  The actual values being 0 and -1. pi here is a symbolic constant.\n\
\n\
Symbolic Constants\n\
\n\
  The following symbolic constants are supported,\n\
  \n\
    pi    -->  3.14159265358979323846\n\
    e     -->  2.7182818284590452354\n\
    inf   -->  infinity\n\
    rand  -->  random double in the range (0, 1) excluding 0 and 1. (ironic, i know...)\n\
  \n\
  For example,\n\
  \n\
    ./calc 'atan inf' 'rand, sin $1, cos($2 - $1)' '-+_+_++_--~---~+_rand'\n\
    \n\
    1.570796327 0.4473471527,0.4325752577,0.9998908975 0.6601152609\n\
\n\
"

static int USE_DEGREE = 0;
static int USE_NEWLINE = 0;

static int resultCount;
static double* results;

static int inCount;
static struct token* inQueue;

static int opCount;
static struct token* opStack;

static int outCount;
static double* outStack;

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
	
	printf("calc: Expression missing.\nUsage: calc [OPTIONS] EXPRESSIONS.\nTry 'calc --help' for more information.");
	exit(-1);
}

int tokenize(char* expr) {
	
	int i;
	char c;
	char* s;
	double d;
	struct token temp;
	
	while((c = tolower(*expr++)) != '\0') {
		
		if(isspace(c))
			continue;
		
		if(isalpha(c)) {
			i = 0;
			s = (char*) malloc((strlen(expr) + 2) * sizeof(char));
			
			do {
				*(s + i++) = c;
			} while(isalpha(c = tolower(*expr++)));
			
			*(s + i) = '\0';
			
			if(isFunction(s)) {
				temp.type = FUNCTION;
				temp.data.func = s;
				temp.precedence = 3;
				temp.leftAssociative = 0;
			}
			
			else if(isSymbol(s)) {
				temp.type = CONSTANT;
				temp.data.d = getSymbol(s);
			}
			
			else {
				printf("Undefined function/symbol. %s", s);
				return 0;
			}
			
			--expr;
		}
		
		else if(isdigit(c) || c == '.') {
			errno = 0;
			d = strtod(--expr, &s);
			
			if(expr == (char*) s) {
				printf("Invalid constant.");
				return 0;
			}
			
			if(errno != 0) {
				printf("Constant to large for double.");
				return 0;
			}
			
			temp.type = CONSTANT;
			temp.data.d = d;
			
			expr = (char*) s;
		}
		
		else if(c == '(' || c == ')') {
			temp.type = PARENTHESIS;
			temp.data.op = c;
		}
		
		else if(c == ',') {
			temp.type = SEPARATOR;
			temp.data.op = c;
		}
		
		else if(isOperator(c)) {
			temp.type = OPERATOR;
			temp.data.op = c;
			temp.precedence = precedence(c);
			temp.leftAssociative = isLeftAssociative(c);
		}
		
		else {
			printf("Invalid token. %c", c);
			return 0;
		}
		
		inQueue[inCount++] = temp;
	}
	
	return 1;
}

int execute(struct token temp) {
	
	double d1, d2, result;
	
	if(temp.type == OPERATOR) {
		if(isBinary(temp.data.op) == 1) {
			if(outCount < 2) {
				printf("Malformed Expression.");
				return 0;
			}
			
			d2 = outStack[--outCount];
			d1 = outStack[--outCount];
			
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
				printf("Malformed Expression.");
				return 0;
			}
			
			d1 = outStack[--outCount];
			
			switch(temp.data.op) {
				case '_':
					result = d1;
					break;
				case '~':
					result = -d1;
					break;
				case '!':
					if(floor(d1) != d1 || d1 < 0.0 || d1 == INFINITY) {
						printf("Factorial is only defined for natural numbers.");
						return 0;
					}
					
					result = factorial(d1);
					break;
				case '$':
					if(floor(d1) != d1 || d1 < 0 || d1 > resultCount) {
						printf("Invalid result index.");
						return 0;
					}
					
					result = results[(int) d1 - 1];
					break;
			}
		}
	}
	
	else {
		if(outCount < 1) {
			printf("Malformed Expression.");
			return 0;
		}
		
		d1 = outStack[--outCount];
		
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
		
		else if(strcmp(temp.data.func, "log") == 0)
			result = log10(d1);
		
		else if(strcmp(temp.data.func, "ln") == 0)
			result = log(d1);
		
		else if(strcmp(temp.data.func, "exp") == 0)
			result = exp(d1);
		
		else if(strcmp(temp.data.func, "round") == 0)
			result = round(d1);
		
		else if(strcmp(temp.data.func, "ceil") == 0)
			result = ceil(d1);
		
		else if(strcmp(temp.data.func, "floor") == 0)
			result = floor(d1);
		
		else if(strcmp(temp.data.func, "sqrt") == 0)
			result = sqrt(d1);
		
		else if(strcmp(temp.data.func, "abs") == 0)
			result = fabs(d1);
		
		else if(strcmp(temp.data.func, "sgn") == 0)
			result = d1 > 0 ? 1 : (d1 < 0 ? -1 : 0);
	}
	
	outStack[outCount++] = result;
	
	return 1;
}

int emptyOpStack() {
	
	while(opCount != 0) {
		if(opStack[opCount - 1].data.op == '(') {
			printf("Mismatched '('.");
			return 0;
		}
		
		if(!execute(opStack[--opCount]))
			return 0;
	}
	
	if(outCount != 1) {
		printf("Malformed expression.");
		return 0;
	}
	
	*(results + resultCount++) = outStack[--outCount];
	return 1;
}

void shuntYard() {
	
	int unary = 1;
	struct token temp;
	
	for(int i = 0; i < inCount; i++) {
		temp = inQueue[i];
		
		if(temp.type == CONSTANT)
			outStack[outCount++] = temp.data.d;
		
		else if(temp.type == PARENTHESIS) {
			if(temp.data.op == '(') {
				opStack[opCount++] = temp;
			}
			
			else if(temp.data.op == ')') {
				
				while(1) {
					if(opCount == 0) {
						printf("Mismatched ')'.");
						return;
					}
					
					temp = opStack[--opCount];
					
					if(temp.data.op == '(')
						break;
					
					if(temp.type == CONSTANT)
						outStack[outCount++] = temp.data.d;
					else
						if(!execute(temp))
							return;
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
			
			if(temp.leftAssociative) {
				while(opCount != 0 && (opStack[opCount - 1].type == OPERATOR || opStack[opCount - 1].type == FUNCTION) && temp.precedence <= opStack[opCount - 1].precedence)
					if(!execute(opStack[--opCount]))
						return;
			}
			
			else {
				while(opCount != 0 && (opStack[opCount - 1].type == OPERATOR || opStack[opCount - 1].type == FUNCTION) && temp.precedence < opStack[opCount - 1].precedence)
					if(!execute(opStack[--opCount]))
						return;
			}
			
			opStack[opCount++] = temp;
		}
		
		else if(temp.type == SEPARATOR) {
			if(!emptyOpStack())
				return;
		}
		
		else {
			opStack[opCount++] = temp;
		}
		
		unary = !(temp.type == CONSTANT || (temp.type == PARENTHESIS && temp.data.op == ')'));
	}
	
	if(!emptyOpStack())
		return;
	
	for(int i = 0; i < resultCount; i++)
		printf("%.10G%s", *(results + i), resultCount - i == 1 ? "" : ",");
}

void evaluate(char* expr, int addEndChar) {
	
	inCount = 0;
	inQueue = (struct token*) malloc(strlen(expr) * sizeof(struct token));
	
	opCount = 0;
	opStack = (struct token*) malloc(strlen(expr) * sizeof(struct token));
	
	outCount = 0;
	outStack = (double*) malloc(strlen(expr) * sizeof(double));
	
	resultCount = 0;
	results = (double*) malloc(strlen(expr) * sizeof(double));
	
	if(tokenize(expr))
		shuntYard();
	
	printf("%s", addEndChar ? (USE_NEWLINE ? "\n" : " ") : "");
	
	free(inQueue);
	free(opStack);
	free(outStack);
	free(results);
}
