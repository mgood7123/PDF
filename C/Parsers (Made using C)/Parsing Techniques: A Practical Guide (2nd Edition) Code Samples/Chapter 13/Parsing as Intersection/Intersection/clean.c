#include	"cfg-fsa.h"
#include	"grammar.h"
#include	"clean.h"
#include	"free.h"
#include	"print.h"

/*
  The recommended way of cleaning up a grammar is:
  1. remove all non-productive rules;
  2. remove all non-reachable non-terminals with their rules.

  Unfortunately our data structure (grammar.h) does not allow us to do
  step 1: rules deriving from the same original rule are not
  represented separately but rather as trees, and so they cannot be
  marked and/or removed separately.

  We solve this by first removing all non-productive non-terminals
  (which we can do).  This leaves us with trees for productive
  non-terminals that may contain non-productive subtrees.  This
  non-productivity can stem only from undefined symbols, since all
  defined non-terminals are now productive. So we achieve the effect
  of step 1 by searching the trees and remove all subtrees that are
  headed by an undefined symbol.

  Step 2 poses no problems, and step 1 does not need to be repeated.
*/

static int its_modified;		/* for various transitive closures */

							/* REMOVE USELESS */
static void
remove_unmarked_symbols(void) {
	struct symbol **hook = &its;

	while (*hook) {
		struct symbol *lhs = *hook;

		if (lhs->mark) {
			/* leave and clear the flag */
			lhs->mark = 0;
			hook = &lhs->down;
		}
		else {
			/* remove */
			*hook = lhs->down;
			lhs->down = 0;
			free_tree(lhs);
		}
	}
}

static int
useful_symbol(struct symbol *s) {
	struct symbol *lhs;

	if (!s->in_fsa) return 1;

	for (lhs = its; lhs; lhs = lhs->down) {
		if (symbols_equal(lhs, s) && lhs->mark) return 1;
	}

	return 0;
}

static int
useful_subtree(struct symbol *s) {
	if (s == 0) return 1;

	for (s = s; s; s = s->down) {
		if (useful_symbol(s) && useful_subtree(s->next)) return 1;
	}
	
	return 0;
}

static void
mark_useful_lhss(void) {
	struct symbol *lhs;

	for (lhs= its; lhs; lhs = lhs->down) {
		if (!lhs->mark && useful_subtree(lhs->next)) {
			lhs->mark = 1;
			its_modified = 1;
		}
	}
}

static void
mark_useful_symbols(void) {
	/* transitive closure */
	its_modified = 1;

	while (its_modified) {
		its_modified = 0;
		mark_useful_lhss();
	}
}

static void
remove_useless_symbols(void) {
	mark_useful_symbols();
	if (v_flag) {
		print_grammar(CFG, "Useful symbols marked", its);
	}
	remove_unmarked_symbols();
}

							/* REMOVE UNDEFINED */
static int
is_defined(struct symbol *s) {
	struct symbol *lhs;

	if (!s->in_fsa) return 1;

	for (lhs = its; lhs; lhs = lhs->down) {
		if (symbols_equal(lhs, s)) return 1;
	}

	return 0;
}

static void
remove_undefined_subtrees(struct symbol **hook) {
	/* I can't think of a way to make the copied code go away:-( */

	while (*hook) {
		struct symbol *s = *hook;

		if (!is_defined(s)) {
			/* remove s */
			*hook = s->down;	/* detach s from main tree */
			s->down = 0;		/* detach main tree from s */
			free_tree(s);
			its_modified = 1;
			continue;
		}

		if (s->next) {
			/* there is a next */
			remove_undefined_subtrees(&s->next);
			if (s->next == 0) {
				/* but now it's gone, so remove s as well */
				*hook = s->down;/* detach s from main tree */
				s->down = 0;	/* detach main tree from s */
				free_tree(s);
				its_modified = 1;
				continue;
			}
		}

		hook = &s->down;
	}
}

static void
remove_undefined_rules(void) {
	remove_undefined_subtrees(&its);
}

							/* REMOVE UNREACHABLES */

static void
mark_rules_for(struct symbol *s) {
	struct symbol *lhs;

	for (lhs= its; lhs; lhs = lhs->down) {
		if (!lhs->mark && symbols_equal(lhs, s)) {
			lhs->mark = 1;
			its_modified = 1;
		}
	}
}

static void
mark_reachable_rhs(struct symbol *s) {
	if (s == 0) return;

	mark_rules_for(s);
	mark_reachable_rhs(s->next);
	mark_reachable_rhs(s->down);
}

static void
mark_reachable_rhss(void) {
	struct symbol *lhs;

	for (lhs= its; lhs; lhs = lhs->down) {
		if (lhs->mark == 1) {
			mark_reachable_rhs(lhs->next);
		}
	}
}

static void
mark_reachable_symbols(void) {
	struct symbol *lhs;

	/* initially, only the start symbols are reachable */
	for (lhs= its; lhs; lhs = lhs->down) {
		if (is_start_name(lhs->name) && !lhs->in_fsa) {
			lhs->mark = 1;
		}
	}

	/* transitive closure */
	its_modified = 1;

	while (its_modified) {
		its_modified = 0;
		mark_reachable_rhss();
	}
}

static void
remove_unreachable_symbols(void) {
	mark_reachable_symbols();
	if (v_flag) {
		print_grammar(CFG, "Reachable symbols marked", its);
	}
	remove_unmarked_symbols();
}

#ifdef	XXXX
/* side-tracked; see grammar.h */
static void
remove_undefined_roots(void) {
	int i, j;

	/* keep from the start names ... */
	for (i = 0, j = 0; start_names[i]; i++) {
		const struct symbol *s = its;

		while (s) {
			/* ... those that are defined */
			if (s->name == start_names[i]) {
				start_names[j++] = start_names[i];
				break;
			}
			s = s->down;
		}
	}
	start_names[j] = 0;
}
#endif
							/* CLEAN */
void
clean(void) {
	/* for an explanation see comment at top of file */
	remove_useless_symbols();
	if (v_flag) {
		print_grammar(CFG, "Useless symbols removed", its);
	}
	remove_undefined_rules();
	/*	remove_undefined_roots();*/
	if (v_flag) {
		print_grammar(CFG, "Rules with undefined symbols removed", its);
	}
	remove_unreachable_symbols();
	if (v_flag) {
		print_grammar(CFG, "Unreachable symbols removed", its);
	}
}
