#include <stdio.h>

#include "calc.h"

int main(int argc, char* argv[]) {
	
	int i = parseOptions(argc, argv);
	
	for(; i < argc; i++)
		evaluate(argv[i], argc - i != 1);
	
	printf("\n");
	
	return 0;
}
