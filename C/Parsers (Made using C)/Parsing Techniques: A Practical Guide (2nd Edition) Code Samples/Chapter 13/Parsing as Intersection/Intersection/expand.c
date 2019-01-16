#include	<stdio.h>

#include	"cfg-fsa.h"
#include	"grammar.h"
#include	"expand.h"
#include	"print.h"
#include	"clean.h"

static struct symbol **its_hook;
static int expand_count;

static void
expand_root(const char *root_name, int pre, int post) {
	/* adds "root_name -> root_name_pre_post" */

	/* create a non-marked lhs for the root */
	struct symbol *lhs = new_symbol(root_name);

	/* connect the lhs to a marked rhs for the root */
	lhs->next = new_marked_symbol(root_name, pre, post);

	/* link the lhs to the its */
	*its_hook = lhs;
	its_hook = &lhs->down;

	expand_count++;
}

static void
expand_roots(void) {
	int r;

	for (r = 0; start_names[r]; r++) {
		const char *root_name = start_names[r];
		int i;
		
		for (i = min_state; i <= max_state; i++) {
			int j;

			for (j = min_state; j <= max_state; j++) {
				if (is_start_state(i) && is_end_state(j)) {
					expand_root(root_name, i, j);
				}
			}
		}
	}
}

/*	expand_lhs() and expand_rhs() are very similar, but still too
	different to be merged, I think.

	Given the rule P -> A ... M ... Z, expand_lhs(P, l) should produce
		lPr lA ... M ... Zr
	where r ranges over all states, and call expand_rhs(A, l, r),
	whereas expand_rhs(M, l, r) should produce
		lMx xN ... Zr
	where x ranges over all states, and call expand_rhs(N, x, r).
	
	Also, expand_lhs attaches its nodes to its_hook, expand_rhs to a
	hook passed as a parameter.

	So the generation mechanisms differ and the parameters are used in
	different ways.  Of course this can all be patched together, but not
	in a conceptually illuminating way, as far as I can see.
*/

static void
expand_rhs(
	struct symbol *source,
	struct symbol **hook,
	int pre_state,
	int last_state
) {
	if (source->next == 0) {
		/* we have arrived at the last member */
		struct symbol *s =
			new_marked_symbol(source->name, pre_state, last_state);

		*hook = s;
		hook = &s->down;	/* actually does nothing */
		
		expand_count++;
	}
	else {
		int j;

		for (j = min_state; j <= max_state; j++) {
			struct symbol *s =
				new_marked_symbol(source->name, pre_state, j);

			*hook = s;
			hook = &s->down;

			expand_rhs(source->next, &s->next, j, last_state);
		}
	}
}

static void
expand_lhs(
	struct symbol *source,
	int pre_state
) {
	/* adds "source_name_pre_X -> ..." for all states X */

	if (source->next == 0) {
		/* it is an epsilon rule; it spans nothing */
		struct symbol *lhs =
			new_marked_symbol(source->name, pre_state, pre_state);

		*its_hook = lhs;
		its_hook = &lhs->down;

		expand_count++;
	}
	else {		
		int j;

		for (j = min_state; j <= max_state; j++) {
			struct symbol *lhs =
				new_marked_symbol(source->name, pre_state, j);

			*its_hook = lhs;
			its_hook = &lhs->down;

			expand_rhs(source->next, &lhs->next, pre_state, j);
		}
	}
}

static void
expand_rule(struct symbol *source) {
	int pre_state;

	for (pre_state = min_state; pre_state <= max_state; pre_state++) {
		expand_lhs(source, pre_state);
	}
}

static void
expand_grammar(void) {
	struct symbol *rule;

	for (rule = grammar; rule; rule = rule->down) {
		expand_rule(rule);
	}

	if (v_flag) {
		printf("number of expanded rules = %d\n", expand_count);
	}
}

static void
expand_transition(struct symbol *source) {
	/* adds "token_pre_post -> token" */

	/* create a marked lhs for the token */
	struct symbol *lhs =
		new_marked_symbol(source->name, source->pre, source->post);

	/* connect the lhs to a non-marked rhs for the token */
	lhs->next = new_symbol(source->name);

	/* link the lhs to the its */
	*its_hook = lhs;
	its_hook = &lhs->down;
}

static void
expand_fsa(void) {
	struct symbol *trans;

	for (trans = fsa; trans; trans = trans->down) {
		expand_transition(trans);
	}
}

void
intersect(void) {
	its_hook = &its;
	expand_count = 0;

	expand_roots();
	expand_grammar();
	expand_fsa();

	if (v_flag) {
		print_grammar(CFG, "Expanded grammar", its);
	}
	clean();
}
