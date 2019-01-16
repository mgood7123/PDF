/*	This file is part of the CS sentence generator CSgen
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: grammar.c,v 1.5 2016-09-19 08:26:15 Gebruiker Exp $
*/

#include	<stdio.h>
#include	<stdlib.h>

#include	"gram.h"
#include	"grammar.h"
#include	"error.h"
#include	"string.h"

/* When a grammar is read in, symbols are looked up in a symbol list and
   replaced by unique pointers. So symbols can be compared by just comparing
   pointers.
   This is efficient not only at run time, but also when writing code.

   When the grammar is built in, GCC gives equal strings the same addresses,
   so symbols can again be compared by just comparing pointers, but it is far
   from certain that other compilers act likewise.

   So there is a routine read_grammar() which either reads in the grammar
   (not implemented), or checks that the compiler indeed merges equal strings.
   (There is also left-over code for the case that the compiler does not merge
   equal strings. It can be activated by defining a macro MERGE.)
*/

static void
check_for_duplicate_terminal_symbols(void) {
	int i;
	for (i = 0; terminal[i]; i++) {
		int j;
		for (j = i+1; terminal[j]; j++) {
			if (terminal[i] == terminal[j])
				error("grammar", "duplicate terminal symbols");
		}
	}
}

int
is_terminal(const char *symbol) {
	int i;
	for (i = 0; terminal[i]; i++) {
		if (terminal[i] == symbol) return 1;
	}
	return 0;
}

#ifdef	MERGE
static void
substitute_in_string(const char **str, const char *symbol) {
	int i;
	for (i = 0; str[i]; i++) {
		if (strcmp(str[i], symbol) == 0) str[i] = symbol;
	}
}

static void
substitute_in_grammar(const char *symbol) {
	/* replace all addresses of the string 'symbol' by the address of this
	   copy of 'symbol'
	*/
	int i;
	for (i = 0; rule[i].lhs[0]; i++) {
		struct rule *r = &rule[i];
		substitute_in_string(r->lhs, symbol);
		substitute_in_string(r->rhs, symbol);
	}
}
#endif

static void
substitue_symbols(void) {
#ifdef	MERGE
	int i;
	for (i = 0; terminal[i]; i++) {
		substitute_in_grammar(terminal[i]);
	}
	/* this code replaces the addresses of all occurrences of a non-terminal
	   by the address of the last occurrence of that non-terminal
	*/
	for (i = 0; rule[i].lhs[0]; i++) {
		struct rule *r = &rule[i];
		int j;
		for (j = 0; r->lhs[j]; j++) {
			if (!is_terminal(r->lhs[j]))
			    substitute_in_grammar(r->lhs[j]);
		}
		for (j = 0; r->rhs[j]; j++) {
			if (!is_terminal(r->rhs[j]))
			    substitute_in_grammar(r->rhs[j]);
		}
	}
#endif
}

void
read_grammar(void) {
	char *a = "S";
	char *b = "S";
	if (a != b) error("program", "compiler does not merge equal strings");
	/* each terminal should be unique */
	check_for_duplicate_terminal_symbols();
	substitue_symbols();
}

void
print_string_of_symbols(const char **a) {
	if (!*a) return;
	printf("%s", *a++);
	while (*a) {
		printf(" %s", *a++);
	}
}

void
print_grammar(void) {
	int i;
	for (i = 0; rule[i].lhs[0]; i++) {
		printf("rule %d: ", i);
		struct rule *r = &rule[i];
		print_string_of_symbols(r->lhs);
		printf(" => ");
		print_string_of_symbols(r->rhs);
		printf("\n");
	}
	printf("\nSF 0 (start string): ");
	print_string_of_symbols(start_string);
	printf("\n");
}
