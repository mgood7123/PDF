/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: table_prec.c,v 1.4 2007/12/21 16:49:48 dick Exp $
*/

/* This version of precedence() uses the tables generated in grammar.i */

#include	<string.h>

#include	"precedence.h"
#include	"error.h"

#include	"grammar.i"

char
precedence(const char *t1, const char *t2) {
	return precedence_from_table(*t1, *t2);
}
