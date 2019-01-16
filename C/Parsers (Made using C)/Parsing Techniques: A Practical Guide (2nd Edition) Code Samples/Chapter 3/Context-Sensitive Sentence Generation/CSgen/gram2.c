/*	This file is part of the CS sentence generator CSgen
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: gram2.c,v 1.1 2016-09-19 08:26:14 Gebruiker Exp $
*/

/* like gram0.c, demonstrating reverse grammar use */

#include	"gram.h"

const char *terminal[] = {"a", "b", "c", 0};

struct rule rule[] = {
	{{"S"}, {"a", "b", "c"}},
	{{"S"}, {"a", "S", "Q"}},
	{{"b", "Q", "c"}, {"b", "b", "c", "c"}},
	{{"c", "Q"}, {"Q", "c"}},
	0
};

const char *start_string[] = {"a", "a", "b", "b", "c", "c", 0};
