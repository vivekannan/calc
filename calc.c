#include <stdio.h>
#include <stdlib.h>

#include "struct.h"
#include "helpers.h"

int main(int argc, char* argv[]) {
	
	int i = parseOptions(argc, argv);
	
	for(; i < argc; i++)
		evaluate(argv[i]);
	
	return 0;
}
