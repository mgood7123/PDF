#include	"vector.h"
#include	"grammar.h"
#include	"parse.h"
#include	"report.h"

#include	<stdio.h>

int
main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Call is: parser input_string\n");
		return 1;
	}

	InitializeGrammarTables();
	Parse(argv[1]);
	PrintParse();
	PrintParseTree();

	return 0;
}
