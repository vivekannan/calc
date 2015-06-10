#include <stdio.h>

#include "calc.h"

int main(int argc, char* argv[]) {
	
	int i = parseOptions(argc, argv);
	int ret = 0;
	
	for(; i < argc; i++)
		if (!evaluate(argv[i], argc - i != 1)) {
			ret = 1;
			break;
		}
	
	printf("\n");
	
	return ret;
}
