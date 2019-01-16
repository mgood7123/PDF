/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: table.c,v 1.3 2002/10/18 12:33:44 dick Exp $
*/

/*
	A very simple, almost naive implementation of a precedence
	parser generator.

	The grammar is supplied in the form of a table in C, in the
	format as shown in the file grammar.c.  Non-terminals are
	strings, terminals are strings of length 1.  The list of rules
	must be terminated by a 0.

	The file $(GRAMMAR).c is compiled into the parser table
	generator, and a call produces C code for
		a #define N_TOKENS
		an array tokens[]
		a 2D array precedence_table[][]
		a routine precedence_from_table(char t1, char t1)

	An example of a precedence parser can be found in parser.c.
*/

#include	<stdio.h>

#include	"sets.h"
#include	"grammar.h"
#include	"precedence.h"

static int s_flag = 0;				/* try symmetric table */
						/* not very successful */

static const char *token[MAX_SET_SIZE];
static int n_tokens;
static char table[MAX_SET_SIZE][MAX_SET_SIZE];
static int left_map[MAX_SET_SIZE];
static int right_map[MAX_SET_SIZE];

static void
get_tokens(void) {
	int i;

	/* collect the tokens */
	for (i = 0; grammar[i][0]; i++) {
		const char **rule = grammar[i];
		int j;

		for (j = 2; rule[j]; j++) {
			if (is_terminal(rule[j])) {
				add_item_to_set(rule[j], token);
			}
		}
	}

	/* count them */
	n_tokens = 0;
	for (i = 0; token[i]; i++) {
		n_tokens++;
	}
}

static void
get_table(void) {
	int i;

	for (i = 0; i < n_tokens; i++) {
		int j;

		for (j = 0; j < n_tokens; j++) {
			table[i][j] = precedence(token[i], token[j]);
		}
	}
}

static void
swap_map(int map[], int i, int j) {
	int tmp = map[i];
	map[i] = map[j];
	map[j] = tmp;
}

static int
skew(void) {
	int skew_count = 0;
	int i;

	for (i = 0; i < n_tokens; i++) {
		/* check row i */
		const int right_map_i = right_map[i];
		int status = 0;
		int j;

		for (j = 0; j < n_tokens; j++) {
			const int prec = table[right_map_i][left_map[j]];
			const int new_status = (
				prec == '>' ? 0 :
				prec == '=' ? 1 :
				prec == '<' ? 2 :
				status
			);
			
			if (new_status < status) {
				skew_count++;
			}
			else {
				status = new_status;
			}
		}
	}

	for (i = 0; i < n_tokens; i++) {
		/* check column i */
		const int left_map_i = left_map[i];
		int status = 2;
		int j;

		for (j = 0; j < n_tokens; j++) {
			const int prec = table[right_map[j]][left_map_i];
			const int new_status = (
				prec == '>' ? 0 :
				prec == '=' ? 1 :
				prec == '<' ? 2 :
				status
			);
			
			if (new_status > status) {
				skew_count++;
			}
			else {
				status = new_status;
			}
		}
	}

	return skew_count;
}

static int
improve_skew(void) {
	int sk = skew();
	int i;

	for (i = 0; i < n_tokens; i++) {
		int j;

		for (j = i+1; j < n_tokens; j++) {
			swap_map(left_map, i, j);
			if (skew() < sk) return 1;
			swap_map(left_map, i, j);

			swap_map(right_map, i, j);
			if (skew() < sk) return 1;
			swap_map(right_map, i, j);
		}
	}
	return 0;
}

static void
symmetric_table(void) {
	/* not very successful */
	while (improve_skew()) {
	}
}

static void
get_map(void) {
	int i;

	for (i = 0; i < n_tokens; i++) {
		left_map[i] = i;
		right_map[i] = i;
	}

	if (s_flag) {
		symmetric_table();
	}
}

static void
generate_token_definition(void) {
	printf("#define\tN_TOKENS\t%d\n", n_tokens);
}

static void
generate_precedence_table_definition(void) {
	int i;

	printf("static const char precedence_table[][N_TOKENS+1] = {\n");
	/* top row shows left_map[] */
	printf("\t{' '");
	for (i = 0; i < n_tokens; i++) {
		printf(", '%s'", token[left_map[i]]);
	}
	printf("},\n\n");

	for (i = 0; i < n_tokens; i++) {
		const int right_map_i = right_map[i];
		int j;

		/* left margin column shows right_map[] */
		printf("\t{'%s'", token[right_map_i]);
		for (j = 0; j < n_tokens; j++) {
			printf(", '%c'", table[right_map_i][left_map[j]]);
		}
		printf("},\n");
	}
	printf("\t{0}\n};\n\n");
}

static void
generate_precedence_routine(void) {
	printf("static char\n");
	printf("precedence_from_table(char t1, char t2) {\n");
	printf("\tint i;\n");
	printf("\n");
	printf("\tfor(i = 1; i < N_TOKENS+1; i++) {\n");
	printf("\t\tif (precedence_table[i][0] == t1) {\n");
	printf("\t\t\tint j;\n");
	printf("\n");
	printf("\t\t\tfor(j = 1; j < N_TOKENS+1; j++) {\n");
	printf("\t\t\t\tif (precedence_table[0][j] == t2)\n");
	printf("\t\t\t\t\treturn precedence_table[i][j];\n");
	printf("\t\t\t}\n");
	printf("\t\t}\n");
	printf("\t}\n");
	printf("\treturn ' ';\n");
	printf("}\n");
}

int
main(int argc, char *argv[]) {
	if (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 's':
			s_flag = 1;
			break;
		}
		argc--, argv++;
	}

	get_tokens();
	get_table();
	get_map();

	generate_token_definition();
	generate_precedence_table_definition();
	generate_precedence_routine();

	return 0;
}
