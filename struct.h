#define CONSTANT 0
#define OPERATOR 1
#define FUNCTION 2
#define PARENTHESIS 3

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