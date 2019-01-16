/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: precedence.h,v 1.1 2002/09/02 09:11:59 dick Exp $
*/

extern char precedence(const char *t1, const char *t2);
/* computes the precedence relation of t1 and t2 straight from the grammar */
