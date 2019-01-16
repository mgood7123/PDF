#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>

#include	"grammar.h"
#include	"read.h"
#include	"token.h"
#include	"error.h"

							/* GRAMMAR */
static void
insert_name_in(const char *list[], const char *n) {
	int i;

	for (i = 0; list[i]; i++) {
		if (list[i] == n) return;
	}
	list[i] = n;
}

static void
delete_name_from(const char *list[], const char *n) {
	int i;

	for (i = 0; list[i]; i++) {
		if (list[i] == n) break;
	}
	for (i = i; list[i]; i++) {
		list[i] = list[i+1];
	}
}

static void
define_nonterminal(const char *n) {
	if (name_in(terminals, n)) {
		delete_name_from(terminals, n);
	}
	insert_name_in(nonterminals, n);
}

static void
apply_symbol(const char *n) {
	if (name_in(nonterminals, n)) return;
	insert_name_in(terminals, n);
}

static void
check_symbol_name(const char *n) {
	if (n == stored_token("?"))
		error_and_exit("catch-all symbol ? not allowed in grammar");
}

int
read_grammar(void) {
	struct symbol **gr_hook;
	int i;

	line_number = 0;

	/* read the start symbols into start_names[] */
	if (get_token_line() != line_OK) return 0;
	for (i = 0; i < token_ptr; i++) {
		check_symbol_name(token[i]);
		start_names[i] = token[i];
	}
	start_names[token_ptr] = 0;

	/* read the rules */
	grammar = 0;
	gr_hook = &grammar;
	while (get_token_line() == line_OK) {
		int ptr = 0;
		struct symbol **rhs_hook;
		struct symbol *s;

		/* the lhs */
		*gr_hook = s = new_symbol(token[0]);
		gr_hook = &s->down;

		check_symbol_name(s->name);
		define_nonterminal(s->name);

		if (token_ptr < 2 || token[1] != stored_token("->"))
			error_and_exit("no ->");

		/* the rhs */
		rhs_hook = &s->next;
		for (ptr = 2; ptr < token_ptr; ptr++) {
			*rhs_hook = s = new_symbol(token[ptr]);
			rhs_hook = &s->next;

			check_symbol_name(s->name);
			apply_symbol(s->name);
		}
	}
	return 1;
}

							/* FSA */
static int
token2int(const char *tk) {
	const char *p = tk;

	while (*p) {
		if (!isdigit(*p))
			error_and_exit("state number contains non-digit");
		p++;
	}

	return atoi(tk);
}

static void
register_state(int s) {
	if (min_state < 0 || min_state > s) {
		min_state = s;
	}
	if (max_state < s) {
		max_state = s;
	}
}

static void
read_states(int states[]) {
	int i;

	for (i = 0; i < token_ptr; i++) {
		if (i == MAX_STATES-1)
			error_and_exit("too many start or end states");
		states[i] = token2int(token[i]);
	}
	states[i] = -1;
}

static int anonymous_state;

static void
read_transitions(void) {
	struct symbol **fsa_hook;

	fsa_hook = &fsa;
	anonymous_state = 0;
	while (get_token_line() == line_OK) {
		/* the format of this line is <state#> <token>+ <state#> */
		int pre;
		int post;
		struct symbol *s = 0;	/* initialized to satisfy lint */
		int ptr = 0;

		/* get the pre state */
		pre = token2int(token[0]);
		register_state(pre);

		/* now require at least one token and the post state */
		if (token_ptr < 3)
			error_and_exit("no symbol in FSA line");

		/* The follwoing loop body gets executed token_ptr-2 times,
		   which is at least once ...
		*/
		for (ptr = 1; ptr < token_ptr-1; ptr++) {
			*fsa_hook = s =
				new_marked_symbol(
					token[ptr], pre, --anonymous_state
				);
			fsa_hook = &s->down;

			pre = s->post;		/* for next terminal */
		}
		/* ... so s is guaranteed to point to a struct symbol ... */

		/* read the post state */
		post = token2int(token[token_ptr-1]);
		register_state(post);

		/* ... and this assignment is safe */
		s->post = post;			/* overwrite last end state */
		anonymous_state++;		/* cancel last anonymous st. */
	}
}

static void
expand_jokers(void) {
	struct symbol *s;

	for (s = fsa; s; s = s->down) {
		if (s->name == stored_token("?")) {
			int pre = s->pre;
			int post = s->post;
			int i;

			/* we cannot remove the node s, so we reuse it
			   for the first terminal
			*/
			s->name = terminals[0];

			/* and we create new nodes for the other terminals */
			for (i = 1; terminals[i]; i++) {
				/* insert a new node between s and s->down */
				struct symbol *t =
					new_marked_symbol(
						terminals[i], pre, post
					);

				t->down = s->down;
				s->down = t;

				s = t;
			}
		}
	}
}

static void
resolve_anonymous_states(void) {
	struct symbol *s;

	for (s = fsa; s; s = s->down) {
		if (s->pre < 0) {
			s->pre = max_state - s->pre;
		}
		if (s->post < 0) {
			s->post = max_state - s->post;
		}
	}
	max_state += (-anonymous_state);
}

static void
postprocess_fsa(void) {
	expand_jokers();
	resolve_anonymous_states();
}

int
read_fsa(void) {
	fsa = 0;
	min_state = -1;
	max_state = 0;

	switch (get_token_line()) {
	case end_of_file: return 0;		/* no FSA */
	case end_of_section: return 1;		/* empty FSA */
	case line_OK: break;
	}

	/* read the start state */
	read_states(start_states);

	/* read the end states */
	if (get_token_line() != line_OK) error_and_exit("no end state(s)");
	read_states(end_states);

	/* read the transitions */
	read_transitions();
	postprocess_fsa();

	return 1;
}
