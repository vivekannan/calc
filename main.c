#include "helpers.h"

int main(int argc, char* argv[]) {
	
	int i = parseOptions(argc, argv);
	
	for(; i < argc; i++)
		evaluate(argv[i]);
	
	printf("%c", USE_NEWLINE ? '\0' : '\n');
	
	return 0;
}
