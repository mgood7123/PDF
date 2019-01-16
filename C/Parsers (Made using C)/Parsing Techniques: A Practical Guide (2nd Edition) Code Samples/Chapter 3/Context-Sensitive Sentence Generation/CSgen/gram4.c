/*	This file is part of the CS sentence generator CSgen
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: gram4.c,v 1.1 2016-09-19 08:26:15 Gebruiker Exp $
*/

#include	"gram.h"

/* Sheep in Mountain Cleft */

const char *terminal[] = {"=", 0};

struct rule rule[] = {
	{{" ", "<"}, {"<", " "}},		/* one sheep goes left */
	{{">", " "}, {" ", ">"}},		/* one sheep goes right */
	{{" ", ">", "<"}, {"<", ">", " "}},	/* one sheep jumps left */
	{{">", "<", " "}, {" ", "<", ">"}},	/* one sheep jumps right */

	{{"<", "<", " ", ">", ">"}, {"="}},
	0
};

const char *start_string[] = {">", ">", " ", "<", "<", 0};
