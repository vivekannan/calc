#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int USE_DEGREE = 0;
static int USE_DOUBLE = 0;
static int USE_NEWLINE = 0;
static char* HELP_TEXT = "This is help.";

void printMessage(char* message, int exitCode) {
	
	printf("%s\n", message);
	exit(exitCode);
}

int parseOptions(int argc, char* argv[]) {
	
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-d") == 0)
			USE_DOUBLE = 1;
		
		else if(strcmp(argv[i], "-n") == 0)
			USE_NEWLINE = 1;
		
		else if(strcmp(argv[i], "--deg") == 0)
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

char* shuntYard(char* expr) {
	
	char c;
	int sp = 0, op = 0;
	int preOperator = 1;
	char* stack = (char*) malloc(strlen(expr) * sizeof(char));
	char* output = (char*) malloc(strlen(expr) * sizeof(char));
	
	while((c = *expr++) != '\0') {
		
		if(isspace(c))
			continue;
		
		else if(isdigit(c)) {
			preOperator = 0;
			output[op++] = c;
		}
		
		else if(c == '(') {
			preOperator = 0;
			stack[sp++] = c;
		}
		
		else if(c == ')') {
			preOperator = 0;
			
			while(stack[--sp] != '(') {
				output[op++] = stack[sp];
				
				if(sp == 0)
					printMessage("Mismatched ')'.", -2);
			}
			stack[sp] = '\0';
		}
		
		else if(isOperator(c)) {
			if(preOperator == 0)
				preOperator = 1;
			
			else if(c == '-')
				c = '#';
			
			else if(c == '+')
				c = '$';
			
			if(leftAssociative(c))
				while(isOperator(stack[--sp]) && precedence(c) <= precedence(stack[sp]))
					output[op++] = stack[sp];
			
			else
				while(isOperator(stack[--sp]) && precedence(c) < precedence(stack[sp]))
					output[op++] = stack[sp];
			
			stack[++sp] = c;
			stack[++sp] = '\0';
		}
		
		else
			printMessage("Unidentfied token.", -2);
	}
	
	while(--sp > -1) {
		if(stack[sp] == '(')
			printMessage("Mismatched '('.", -2);
		
		output[op++] = stack[sp];
	}
	
	return output;
}

int main(int argc, char* argv[]) {
	
	int i = parseOptions(argc, argv);
	
	for(; i < argc; i++)
		printf("%s\n", shuntYard(argv[i]));
	
	return 0;
}
