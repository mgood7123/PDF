#include	<stdio.h>
#include	<stdlib.h>

#include	"read.h"
#include	"error.h"

const char *progname;

void
error_and_exit(const char *msg) {
	if (line_number == 0) {
		fprintf(stderr, "%s: %s\n", progname, msg);
	}
	else {
		fprintf(stderr, "\"%s\", line %d: %s\n",
			iname, line_number, msg
		);
	}
	exit(1);
}
