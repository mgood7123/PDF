#include	"cfg-fsa.h"
#include	"grammar.h"
#include	"restore.h"
#include	"clean.h"
#include	"print.h"

/* removes the unit rules that expand_root() and expand_fsa() have created */

							/* RESTORE ROOT */
static void
restore_root_symbol_in_tree(struct symbol *s) {
	for (s = s; s; s = s->down) {
		if (	is_start_name(s->name)
		&&	s->in_fsa
		&&	is_start_state(s->pre)
		&&	is_end_state(s->post)
		) {
			s->in_fsa = 0;
		}
		restore_root_symbol_in_tree(s->next);
	}
}

static void
promote_root_rules(void) {
	int done = 0;
	
	while (!done) {
		struct symbol **hook = &its;

		done = 1;
		while (*hook) {
			struct symbol *this = *hook;
			struct symbol *next = this->down;
			
			if (	this->in_fsa
			&&	next != 0
			&&	!next->in_fsa
			) {
				/* swap them */
				this->down = next->down;
				next->down = this;
				*hook = next;

				done = 0;
			}
			else {
				hook = &this->down;
			}
		}
	}		
}

static void
restore_root_rules(void) {
	struct symbol *lhs;

	for (lhs = its; lhs; lhs = lhs->down) {
		if (lhs->in_fsa) {
			restore_root_symbol_in_tree(lhs);
		}
	}
	promote_root_rules();
}

							/* RESTORE FSA */
static int
is_terminal(struct symbol *s) {
	/*	a symbol S_n_m is a generated terminal if it is defined as
		S_n_m -> S
	*/
	struct symbol *lhs;

	for (lhs = its; lhs; lhs = lhs->down) {
		if (	symbols_equal(lhs, s)
		&&	lhs->next
		&&	lhs->next->name == lhs->name
		&&	!lhs->next->in_fsa
		&&	lhs->next->next == 0
		) return 1;
	}
	return 0;
}

static void
restore_terminals_in_rhs(struct symbol *s) {
	for (s = s; s; s = s->down) {
		if (is_terminal(s)) {
			s->in_fsa = 0;
		}
		restore_terminals_in_rhs(s->next);
	}
}

static void
restore_fsa_rules(void) {
	struct symbol *lhs;

	for (lhs = its; lhs; lhs = lhs->down) {
		restore_terminals_in_rhs(lhs->next);
	}
}

							/* RESTORE */
void
restore(void) {
	restore_fsa_rules();
	if (v_flag) {
		print_grammar(CFG, "Terminals restored", its);
	}

	restore_root_rules();
	if (v_flag) {
		print_grammar(CFG, "Root rules restored", its);
	}

	clean();
	if (v_flag) {
		print_grammar(CFG, "Cleaned-up after restoration", its);
	}
}
