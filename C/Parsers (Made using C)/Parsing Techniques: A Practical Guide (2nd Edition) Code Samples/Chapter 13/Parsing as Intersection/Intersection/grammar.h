#define	MAX_LINE_LENGTH		256	/* input line */
#define	MAX_SYMBOLS		10000	/* different names in the grammar ?? */
#define	MAX_STATES		100	/* states in FSA ?? */
#define	MAX_STACK_DEPTH		100	/* for recursive rule printing */

struct symbol {
	const char *name;
	struct symbol *next;		/* next member, aka the AND pointer */
	struct symbol *down;		/* next alternative, aka OR pointer */
	int in_fsa;			/* 1 if pre and post are defined */
	int pre;
	int post;
	int mark;			/* a flag */
};

/*
	Although this struct is called "symbol", it is actually a symbol
	occurrence in the grammar, or a node.

	The grammar structure is represented by a tree of these "symbols"
	starting at 'grammar', in which the 'down' pointer from a node points
	to an alternative and the 'next' pointer has two functions:
	
	1. for lhs nodes it points to the corresponding rhs
	2. for rhs nodes it points to the next member

	So the rules
		A -> B C D
		A -> B C E
		B -> B C F
	are repesented as
		A-B-C-D-0
		|     |
		|     E-0
		|
		B-B-C-F-0
	where - is the next pointer, | is the down pointer and 0 is the
	null pointer.
	
	This data structure has no representation for a rhs that is a
	prefix of another rhs:
		A -> B C 
		A -> B C E
	since there is no node from which to point down to E.  This
	could be remedied by having an end-of-rule node:
		A-B-C-.
		      |
		      E.
	but the problem cannot arise in this program, because of the way
	the rules are read.  Each rule in the input is a tree by itself,
	and all rules derived from it have the same length, so no tree
	has branches of different lengths.  Still this is ugly.

	Whether this is the best possible representation is not clear,
	but this is how it's done at the moment.
*/

/*	To do ????
	It would probably be better if the start names were an
	integral part of the grammar, as alternatives of a single
	anonymous start symbol.  To be done; and watch the restore
	feature!

	Actually the whole structure is wrong, since start_names is
	part of both the immutable grammar and the modifiable its.  As
	a result, only the printing routines can select the reachable
	start names, which is bad.
*/

extern struct symbol *grammar;		/* as described above */
extern const char *start_names[];	/* zero-terminated */
extern const char *nonterminals[];
extern const char *terminals[];

extern struct symbol *fsa;		/* list of symbols, linked by down */
extern int start_states[];		/* non-empty; terminated by -1 */
extern int end_states[];		/* non-empty; terminated by -1 */
extern int min_state;
extern int max_state;			/* note: can be == min_state */

extern struct symbol *its;		/* the intersection grammar */

extern struct symbol *new_symbol(const char *name);
extern struct symbol *new_marked_symbol(const char *name, int pre, int post);
extern int symbols_equal(struct symbol *s1, struct symbol *s2);
extern int name_in(const char *list[], const char *n);
extern int is_start_name(const char *nm);
extern int is_start_state(int n);
extern int is_end_state(int n);

