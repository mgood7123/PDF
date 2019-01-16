/*	This file is part of the CS sentence generator CSgen
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: grammar.h,v 1.2 2016-09-16 10:08:44 Gebruiker Exp $
*/

extern void read_grammar(void);
extern void print_grammar(void);
extern int is_terminal(const char *symbol);
extern void print_string_of_symbols(const char **a);
