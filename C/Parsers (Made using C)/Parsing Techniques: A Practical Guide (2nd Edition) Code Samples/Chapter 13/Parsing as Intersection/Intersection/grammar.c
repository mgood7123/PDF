#include	<stdio.h>
#include	<stdlib.h>

#include	"grammar.h"
#include	"error.h"

static void *
Calloc(int size, int n) {
	void *res = calloc(size, n);

	if (res == 0) error_and_exit("out of memory");

	return res;
}

struct symbol *grammar;
/*	MAX_SYMBOLS is certainly enough for these arrays since the number of
	tokens is tested in stored_token().
*/
const char *start_names[MAX_SYMBOLS];
const char *nonterminals[MAX_SYMBOLS];
const char *terminals[MAX_SYMBOLS];

int
name_in(const char *list[], const char *n) {
	int i;

	for (i = 0; list[i]; i++) {
		if (list[i] == n) return 1;
	}
	return 0;
}

struct symbol *fsa;
int start_states[MAX_STATES];
int end_states[MAX_STATES];
int min_state;
int max_state;

struct symbol *its;			/* to be filled by intersect() */

int
is_start_name(const char *nm) {
	return name_in(start_names, nm);
}

struct symbol *
new_symbol(const char *name) {
	/* create a non-marked symbol */
	struct symbol *s =
		(struct symbol *)Calloc(sizeof (struct symbol), 1);

	s->name = name;
	return s;
}

struct symbol *
new_marked_symbol(const char *name, int pre, int post) {
	struct symbol *s = new_symbol(name);
	
	s->in_fsa = 1;
	s->pre = pre;
	s->post = post;

	return s;
}

int
symbols_equal(struct symbol *s1, struct symbol *s2) {
	if (s1->name != s2->name) return 0;
	if (s1->in_fsa != s2->in_fsa) return 0;
	if (!s1->in_fsa) return 1;
	if (s1->pre != s2->pre) return 0;
	if (s1->post != s2->post) return 0;
	return 1;
}

static int
int_in(int n, int array[]) {
	int i;

/*
	printf("array:");
	for (i = 0; array[i] >= 0; i++) {
		printf(" %d", array[i]);
	}
	printf("\n");
*/

	for (i = 0; array[i] >= 0; i++) {
		if (array[i] == n) return 1;
	}
	return 0;
}

int
is_start_state(int n) {
	return int_in(n, start_states);
}

int
is_end_state(int n) {
	return int_in(n, end_states);
}
