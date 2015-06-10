#ifndef STRUCT_H
#define STRUCT_H 1

#define OPERATOR 0
#define FUNCTION 1
#define PARENTHESIS 2

union t {
	
	char op;
	char* func;
};

struct token {
	
	int type;
	union t data;
	
	int precedence;
	int leftAssociative;
};

#endif