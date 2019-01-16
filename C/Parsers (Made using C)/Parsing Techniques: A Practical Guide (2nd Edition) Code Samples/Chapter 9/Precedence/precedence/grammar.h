/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: grammar.h,v 1.2 2002/09/23 10:57:42 dick Exp $
*/

#define	MAX_RHS_LENGTH	4	/* maximum number of RHS members in any rule */

/*
	The grammar is expected as an array of (MAX_RHS_LENGTH+3)-tuples.
	Each tuple has the form {LHS, ":", RHS1, RHS2, ..., 0}.
	The last tuple is followed by a {0}.
*/

extern const char *grammar[][MAX_RHS_LENGTH+3];
extern int is_terminal(const char *symbol);
