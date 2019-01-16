#include	<stdio.h>
#include	<stdlib.h>

#include	"error.h"

void
error(const char *msg) {
	fprintf(stderr, "\nError: %s\n", msg);
	exit(1);
}

