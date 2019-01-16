/*	This file is part of the CS sentence generator CSgen
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: error.c,v 1.2 2016-09-16 10:08:44 Gebruiker Exp $
*/

#include	<stdio.h>
#include	<stdlib.h>

#include	"error.h"

void
error(const char *where, const char *what) {
	printf(">>>> Error in %s: %s\n\n", where, what);
	exit (1);
}
