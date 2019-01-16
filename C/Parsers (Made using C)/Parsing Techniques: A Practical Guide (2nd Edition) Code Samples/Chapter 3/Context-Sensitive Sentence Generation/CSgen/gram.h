/*	This file is part of the CS sentence generator CSgen
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: gram.h,v 1.2 2016-09-16 20:05:35 Gebruiker Exp $
*/

#define	MAX_HS	16

struct rule {
	const char *lhs[MAX_HS];
	const char *rhs[MAX_HS];
};

extern const char *terminal[];
extern struct rule rule[];
extern const char *start_string[];
