#include	<stdio.h>

#include	"grammar.h"
#include	"print.h"
#include	"error.h"

void
print_symbol(const struct symbol *s) {
	printf(" ");
	printf("%s", s->name);
	if (s->in_fsa) {
		printf("_%d_%d", s->pre, s->post);
	}
	printf(" ");
}

static const struct symbol *stack[MAX_STACK_DEPTH];
static int stack_ptr;

static void
push(const struct symbol *s) {
	if (stack_ptr == MAX_STACK_DEPTH)
		error_and_exit("stack overflow during grammar rule printing");
	stack[stack_ptr++] = s;
}

static void
pop(void) {
	stack_ptr--;
}

static void
print_stack(enum print_type type) {
	int p;

	printf(stack[0]->mark ? "!" : " ");
	print_symbol(stack[0]);
	if (type != FSA) {
		printf("->");
	}
	for (p = 1; p < stack_ptr; p++) {
		print_symbol(stack[p]);
	}
	printf("\n");
}

static void
print_rhs(enum print_type type, struct symbol *s) {
	if (s == 0) {
		print_stack(type);
		return;
	}
	while (s) {
		push(s);
		print_rhs(type, s->next);
		pop();
		s = s->down;
	}
}

/* this is a kludge; see grammar.h */
static void
get_defined_start_names(const struct symbol *gr, const char *sn[]) {
	int i, j;

	/* select from the start names ... */
	for (i = 0, j = 0; start_names[i]; i++) {
		const struct symbol *s = gr;

		while (s) {
			/* ... those that are defined */
			if (s->name == start_names[i]) {
				/* and copy them to sn[] */
				sn[j++] = start_names[i];
				break;
			}
			s = s->down;
		}
	}
	sn[j] = 0;
}

static void
print_start_names(const struct symbol *gr) {
	const char *sn[MAX_SYMBOLS];

	get_defined_start_names(gr, sn);

	printf("start symbol%s: ", (sn[0] != 0 && sn[1] == 0 ? "" : "s"));
	if (sn[0] == 0) {
		printf("none");
	}
	else {
		int i;
		
		for (i = 0; sn[i]; i++) {
			printf("%s%s", (i == 0 ? "" : ", "), sn[i]);
		}
	}
	printf("\n");
}

static void
print_states(const char *name, const int states[]) {
	int i;

	printf("%s state%s: ", name, (states[1] < 0 ? "" : "s"));
	for (i = 0; states[i] >= 0; i++) {
		printf("%s%d", (i == 0 ? "" : ", "), states[i]);
	}
	printf("\n");
}

static void
print_rules(enum print_type type, const struct symbol *gr) {
	const struct symbol *rule;

	stack_ptr = 0;
	for (rule = gr; rule; rule = rule->down) {
		push(rule);
		print_rhs(type, rule->next);
		pop();
	}
}

static void
print_list(const char *name, const char *list[]) {
	printf("%s:", name);
	while (*list) {
		printf(" %s", *list++);
	}
	printf("\n");
}

void
print_grammar(enum print_type type, const char *msg, const struct symbol *gr) {
	printf("\n%s:\n", msg);
	switch (type) {
	case FSA:
		print_states("start", start_states);
		print_states("end", end_states);
		printf("lowest state:  %d\n", min_state);
		printf("highest state: %d\n", max_state);
		break;
	case CFG:
		print_start_names(gr);
		break;
	case CFG_ORIG:
		print_start_names(gr);
		print_list("non-terminals", nonterminals);
		print_list("terminals", terminals);
		break;
	}
	print_rules(type, gr);
}
