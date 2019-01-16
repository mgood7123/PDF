/*	This file is part of the CS sentence generator CSgen
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: CSgen.c,v 1.4 2016-09-19 08:26:14 Gebruiker Exp $
*/

#include	<stdio.h>
#include	<stdlib.h>

#include	"gram.h"
#include	"grammar.h"
#include	"production.h"
#include	"error.h"

int
main(int argc, char *argv[]) {
	argc--, argv++;
	while (argc) {
		if (argv[0][0] != '-') goto bad_call;
		switch (argv[0][1]) {
		case 'r': r_flag = 1; break;
		case 's': s_flag = 1; break;
		default: goto bad_call;
		}
		argc--, argv++;
	}

	read_grammar();
	print_grammar();
	fflush(stdout);

	init_production();
	produce();

	return 0;

	bad_call:
error("call", "see READ_ME file");
}
