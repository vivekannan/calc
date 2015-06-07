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
	
	return (strcmp(f, "sin") == 0 || strcmp(f, "cos") == 0 || strcmp(f, "tan") == 0 || strcmp(f, "asin") == 0 || strcmp(f, "acos") == 0 || strcmp(f, "atan") == 0 || strcmp(f, "exp") == 0 || strcmp(f, "floor") == 0 || strcmp(f, "ceil") == 0 || strcmp(f, "round") == 0 || strcmp(f, "log") == 0 || strcmp(f, "ln") == 0 || strcmp(f, "sinh") == 0 || strcmp(f, "cosh") == 0 || strcmp(f, "tanh") || strcmp(f, "sqrt") || strcmp(f, "abs"));
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

int isBinary(char c) {
	
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
}