#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* HELP_TEXT = "This is help.";
static int USE_DOUBLE = 0;
static int USE_NEWLINE = 0;

void printHelp() {
	
	printf("%s\n", HELP_TEXT);
	exit(0);
}

char* parseOptions(int argc, char* argv[]) {
	
	int i;
	
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-d") == 0)
			USE_DOUBLE = 1;
		
		else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
			printHelp();
		
		else if(strcmp(argv[i], "-n") == 0)
			USE_NEWLINE = 1;
		
		else
			return argv[i];
	}
	
	return (char*) 0;
}

int main(int argc, char* argv[]) {
	
	parseOptions(argc, argv);
	
	return 0;
}
