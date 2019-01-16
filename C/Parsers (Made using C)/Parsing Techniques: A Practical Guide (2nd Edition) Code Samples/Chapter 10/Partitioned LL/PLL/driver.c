#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"anbcn.h"
#include	"driver.h"

void
print(const char *s) {
	printf("%s\n", s);
}

int
error(const char *adm) {
	if (strlen(adm) > 1) {
		fprintf(stderr, "one of ");
	}
	fprintf(stderr, "`%s' expected, `%c' found\n", adm, dot);
	exit(1);
	return 0;
}

int dot;

static void
nextchar(void) {
	int c = getchar();

	dot = (c <= 0 || c == '\n' ? '#' : c);
}

void
token(int ch) {
	if (ch == dot) {
		nextchar();
	}
	else {
		char adm[2];

		adm[0] = ch; adm[1] = '\0';
		(void)error(adm);
	}
}

int
main(void) {
	nextchar();
	(void)S();
	token('#');

	return 0;
}
