#ifndef STRUCT_H
#define STRUCT_H 1

#define CONSTANT 0
#define OPERATOR 1
#define FUNCTION 2
#define PARENTHESIS 3
#define SEPARATOR 4

union t {
	
	char op;
	double d;
	char* func;
};

struct token {
	
	int type;
	union t data;
	
	int precedence;
	int leftAssociative;
};

#endif