/*	This file is part of the 2-way PDA interpreter PDA.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: pda.c,v 1.2 2007/12/21 16:49:43 dick Exp $
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"table.h"

#define	MAX_TEXT_SIZE	1024
#define	MAX_STACK_SIZE	1024
#define	MAX_TABLE_SIZE	10240

static int v_flag;			/* verbose output */

static void
fatal(const char *msg) {
	fprintf(stderr, "pda: fatal error: %s\n", msg);
	exit(1);
}

							/* TEXT */

static char text[MAX_TEXT_SIZE];
static int head = 0;
static int text_length;
#define	dot	(text[head])

static int
get_text(void) {
	int ch;

	text_length = 0;
	while (ch = getchar(), (ch > 0 && ch != '\n')) {
		if (text_length == MAX_TEXT_SIZE-2)
			fatal("Text buffer overflow");
		text[text_length++] = ch;
	}
	if (ch <= 0 && text_length == 0) return 0;
	text[text_length++] = '\n';
	text[text_length] = '\0';
	head = 0;

	return 1;
}

							/* STACK */

static char stack[MAX_STACK_SIZE];
static int TOS;			/* Top Of Stack */

static void
clear_stack(void) {
	stack[0] = '\0';
	TOS = 0;
}

static void
push(const char *new) {
	if (TOS + strlen(new) >= MAX_STACK_SIZE) fatal("stack overflow");
	strcpy(&stack[TOS], new);
	TOS += strlen(new);
}

static void
pop(int length) {
	TOS -= length;
	stack[TOS] = '\0';
}

static void
print_stack(void) {
	printf("%s", stack);
}

							/* REPORT */

static char used[MAX_TABLE_SIZE];
static int transition_cnt;

static void
print_transition(const struct transition *tr) {
	printf("\t{\"%s\", \"%s\",\t\t\"%s\", %d,\t\"%s\"},\n",
	       tr->old, tr->input, (tr->new ? tr->new : ""),
	       tr->motion, tr->action
	);
}

static void
print_action(const struct transition *tr) {
	if (tr->action) {
		printf("%s\n", tr->action);
	}
}

static void
print_status(void) {
	int i;

	printf("text = \"");
	for (i = 0; text[i] != '\n'; i++) {
		printf("%c", text[i]);
	}
	printf("\"\n");

	printf("head:  ");
	for (i = -1; i < head; i++) {
		/* trick to print properly when head < 0 */
		printf(" ");
	}
	printf("^\n");

	printf("stack = \"");
	print_stack();
	printf("\"\n\n");
}

static void
init_report(void) {
	int i;

	for (i = 0; transition[i].old; i++) {
		if (i == MAX_TABLE_SIZE) fatal("table overflow");
		used[i] = 0;
	}
}

static void
report(void) {
	int first_used = 1;
	int first_abused = 1;
	int i;

	for (i = 0; transition[i].old; i++) {
		if (used[i] == 0 && transition[i].action[0] != '-') {
			if (first_used) {
				printf("There were unused transitions:\n");
				first_used = 0;
			}
			print_transition(&transition[i]);
		}
	}

	for (i = 0; transition[i].old; i++) {
		if (used[i] == 1 && transition[i].action[0] == '-') {
			if (first_abused) {
				printf("There were unreachable transitions that were used:\n");
				first_abused = 0;
			}
			print_transition(&transition[i]);
		}
	}
}

							/* PDA */

static void
init_pda(void) {
	clear_stack();
	push(initial_state);
}

static int
is_stack_match(const struct transition *tr, int *length_p) {
	const char *pattern = tr->old;
	int TOP, s, p;

	/* matching proceeds from top to bottom / right to left */
	TOP = strlen(pattern);		/* Top Of Pattern */
	s = TOS-1, p = TOP-1;

	while (s >= 0 && p >= 0) {
		if (pattern[p] == '*' && p != 0 && p != TOP) {
			/* joker has the form x*y and matches x...y,
			   where ... does not contain x or y
			*/
			char x = pattern[p-1], y = pattern[p+1];

			for (;;) {
				if (s < 0) return 0;
				if (stack[s] == x) break;
				if (stack[s] == y) return 0;
				/* order of the tests is important if x==y */
				s--;
			}
			/* we have matched the x already, so: */
			p--;
		}
		else {
			if (stack[s] != pattern[p]) return 0;
		}
		s--, p--;
	}
	/* step back to last match */
	s++, p++;

	if (p == 0) {
		*length_p = TOS - s;
		return 1;
	}
	else return 0;
}

static int
is_input_match(const struct transition *tr) {
	switch (tr->input[0]) {
	case '\0': return 1;		/* no match requirements */
	case '[': {
		int i;
		
		for (i = 1; tr->input[i] != ']'; i++) {
			if (tr->input[i] == dot) return 1;
		}
		
		return 0;
	}
	default: return dot == tr->input[0];
	}
}

static const struct transition *
applicable_transition(int *length_p) {
	/* we need to return the length here, since it need not be equal
	   to the length of the old part of the transition
	*/
	const struct transition *tr_found = 0;
	int i;

	for (i = 0; transition[i].old != 0; i++) {
		const struct transition *tr = &transition[i];
		int length;

		if (is_stack_match(tr, &length) && is_input_match(tr)) {
			if (tr_found != 0) {
				print_transition(tr_found);
				print_transition(tr);
				fatal("non-deterministic automaton");
			}
			tr_found = &transition[i], *length_p = length;
			used[i] = 1;
		}			
	}

	if (tr_found == 0) {
		printf("no transition applicable; input rejected\n");
		print_status();
	}

	return tr_found;
}

static void
apply_transition(const struct transition *tr, int length) {
	/* unstack */
	pop(length);

	/* move (or not) */
	head += tr->motion;
	if (head < 0) {
		print_status();
		fatal("head ran off to the left");
	}
	if (head >= text_length) {
		print_status();
		fatal("head ran off to the right");
	}

	/* stack (or not) */
	if (tr->new == 0) return;
	push(tr->new);

	transition_cnt++;
	if (v_flag) {
		print_status();
	}
}

static void
run_pda(void) {
	const struct transition *tr;
	int length;

	transition_cnt = 0;
	if (v_flag) {
		print_status();
	}
	while ((tr = applicable_transition(&length))) {
		print_action(tr);
		apply_transition(tr, length);

		if (tr->new == 0) break;
	}
	printf("Transition count: %d\n", transition_cnt);
	printf("\n");
}		

int
main(int argc, const char *argv[]) {
	if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'v') {
		v_flag = 1;
	}

	init_report();

	while (get_text()) {
		init_pda();
		run_pda();
	}

	report();

	return 0;
}
