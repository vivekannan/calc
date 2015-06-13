#ifndef STRUCT_H
#define STRUCT_H 1

#define OPERATOR 0
#define FUNCTION 1
#define PARENTHESIS 2

struct token {
	
	int type;
	int subType;
	
	int isBinary;
	int precedence;
	int leftAssociative;
};

#endif