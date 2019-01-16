/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: error.c,v 1.2 2007/12/21 16:49:47 dick Exp $
*/

#include	<stdio.h>
#include	<stdlib.h>

#include	"error.h"

void
error(const char *module, const char *msg) {
	error2(module, msg, 0);
}

void
error2(const char *module, const char *msg, void (*report)(void)) {
	fprintf(stderr, ">>>> %s error: %s\n", module, msg);
	if (report) {
		(*report)();
	}
	exit(1);
}
