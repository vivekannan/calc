#include <ctype.h>
#include <errno.h>
#include <string.h>

static int USE_DEGREE = 0;
static int USE_NEWLINE = 0;
static char* HELP_TEXT = "This is help.";

struct stack opStack;

int inCount;
int outCount;
struct token* inQueue;
struct token* outQueue;

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

int precedence(char c) {
	
	switch(c) {
		case '#':
		case '$':
			return 4;
		case '!':
			return 3;
		case '^':
			return 2;
		case '*':
		case '/':
		case '%':
			return 1;
		case '+':
		case '-':
			return 0;
	}
}

int isOperator(char c) {
	
	switch(c) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '^':
		case '!':
		case '#':
		case '$':
			return 1;
		default:
			return 0;
	}
}

int isFunction(char* f) {
	
	return (strcmp(f, "sin") == 0 || strcmp(f, "cos") == 0 || strcmp(f, "tan") == 0 || strcmp(f, "asin") == 0 || strcmp(f, "acos") == 0 || strcmp(f, "atan") == 0 || strcmp(f, "e") == 0 || strcmp(f, "pi") == 0);
}

int leftAssociative(char c) {
	
	switch(c) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			return 1;
		case '^':
		case '!':
		case '#':
		case '$':
			return 0;
	}
}


void tokenize(char* expr) {
	
	char c;
	char* s;
	double d;
	int unary = 1;
	struct token temp;
	
	while((c = *expr++) != '\0') {
		
		if(isspace(c))
			continue;
		
		if(!isOperator(c)) {
			unary = 0;
			
			if(isalpha(c)) {
				s = (char*) malloc((strlen(expr) + 2) * sizeof(char));
				
				do {
					*s++ = c;
				} while((c = *expr++) != '\0' && isalpha(c));
				
				*s = '\0';
				
				if(!isFunction(s))
					printMessage("Undefined Function", -3);
				
				temp.type = FUNCTION;
				temp.data.func = s;
				
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
				unary = 1;
				temp.type = PARENTHESIS;
				temp.data.op = c;
				
				inQueue[inCount++] = temp;
			}
			
			else
				printMessage("Invalid token.", -3);
		}
		
		else {
			if(unary == 0)
				unary = 1;
			
			else if(c == '-')
				c = '#';
			
			else if(c == '+')
				c = '$';
			
			temp.type = OPERATOR;
			temp.data.op = c;
			
			inQueue[inCount++] = temp;
		}
	}
}

void shuntYard() {
	
	struct token temp;
	
	for(int i = 0; i < inCount; i++) {
		temp = inQueue[i];
		
		if(temp.type == CONSTANT)
			outQueue[outCount++] = temp;
		
		else if(temp.type == PARENTHESIS) {
			if(temp.data.op == '(')
				push(&opStack, temp);
			
			else if(temp.data.op == ')') {
				
				while(1) {
					if(opStack.top == -1)
						printMessage("Mismatched ')'.", -2);
					
					temp = pop(&opStack);
					
					if(temp.data.op == '(')
						break;
					
					outQueue[outCount++] = temp;
				}
			}
		}
		
		else if(temp.type == OPERATOR) {
			if(leftAssociative(temp.data.op))
				while(opStack.top != -1 && isOperator(peek(&opStack).data.op) && precedence(temp.data.op) <= precedence(peek(&opStack).data.op))
					outQueue[outCount++] = pop(&opStack);
			
			else
				while(opStack.top != -1 && isOperator(peek(&opStack).data.op) && precedence(temp.data.op) < precedence(peek(&opStack).data.op))
					outQueue[outCount++] = pop(&opStack);
			
			push(&opStack, temp);
		}
	}
	
	while(opStack.top != -1) {
		if(peek(&opStack).data.op == '(')
			printMessage("Mismatched '('.", -2);
		
		outQueue[outCount++] = pop(&opStack);
	}
	
	for(int i = 0; i < outCount; i++) {
		temp = outQueue[i];
		
		if(temp.type == CONSTANT)
			printf("%lf ", temp.data.d);
		
		else if(temp.type == OPERATOR)
			printf("%c ", temp.data.op);
	}
	printf("\n");
}

void evaluate(char* expr) {
	
	opStack = newStack(strlen(expr));
	
	inCount = 0;
	inQueue = (struct token*) malloc(strlen(expr) * sizeof(struct token));
	
	outCount = 0;
	outQueue = (struct token*) malloc(strlen(expr) * sizeof(struct token));
	
	tokenize(expr);
	shuntYard();
}