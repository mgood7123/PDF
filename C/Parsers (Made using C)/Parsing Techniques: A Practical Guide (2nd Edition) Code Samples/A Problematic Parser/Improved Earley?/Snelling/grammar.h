extern char N[MAX_d];			/* the non-terminals */
extern int n_N;				/* number of elements in N */
/* provided the input contains no non-terminals, T[] need not be implemented */

extern char D[MAX_d];			/* the lhss */
extern char C[MAX_d][MAX_j];		/* the rhss */
extern int bar[MAX_d];			/* actually the length of the rhs */
extern int d;				/* number of rules */

extern const char D0;			/* the new start symbol */
extern const char end;			/* the |- from the paper */

extern void print_grammar(void);
