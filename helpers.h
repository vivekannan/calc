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
		case '!':
			return 5;
		case '~':
		case '_':
			return 4;
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
		case '~':
		case '_':
			return 1;
		default:
			return 0;
	}
}

int isFunction(char* f) {
	
	return (strcmp(f, "sin") == 0 || strcmp(f, "cos") == 0 || strcmp(f, "tan") == 0 || strcmp(f, "asin") == 0 || strcmp(f, "acos") == 0 || strcmp(f, "atan") == 0 || strcmp(f, "exp") == 0 || strcmp(f, "floor") == 0 || strcmp(f, "ceil") == 0 || strcmp(f, "round") == 0 || strcmp(f, "log") == 0 || strcmp(f, "ln") == 0);
}

int isLeftAssociative(char c) {
	
	switch(c) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			return 1;
		case '^':
		case '!':
		case '~':
		case '_':
			return 0;
	}
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

void shuntYard() {
	
	int unary = 1;
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
				while(opStack.top != -1 && (peek(&opStack).type == OPERATOR || peek(&opStack).type == FUNCTION) && temp.precedence <= peek(&opStack).precedence)
					outQueue[outCount++] = pop(&opStack);
			
			else
				while(opStack.top != -1 && (peek(&opStack).type == OPERATOR || peek(&opStack).type == FUNCTION) && temp.precedence < peek(&opStack).precedence)
					outQueue[outCount++] = pop(&opStack);
			
			push(&opStack, temp);
		}
		
		else {
			push(&opStack, temp);
		}
		
		unary = !(temp.type == CONSTANT || (temp.type == PARENTHESIS && temp.data.op == ')'));
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
		
		else if(temp.type == FUNCTION)
			printf("%s ", temp.data.func);
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
	
	free(inQueue);
	free(outQueue);
}