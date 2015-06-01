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
};

struct stack {
	
	int top;
	struct token* tokens;
};

struct stack newStack(int size) {
	
	struct stack s;
	
	s.top = -1;
	s.tokens = (struct token*) malloc(size * sizeof(struct token));
	
	return s;
}

struct token peek(struct stack* s) {
	
	return s->tokens[s->top];
}

struct token pop(struct stack* s) {
	
	return s->tokens[s->top--];
}

void push(struct stack* s, struct token t) {
	
	s->tokens[++s->top] = t;
}