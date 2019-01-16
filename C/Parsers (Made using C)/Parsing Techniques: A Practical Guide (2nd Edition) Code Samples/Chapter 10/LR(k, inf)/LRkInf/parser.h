/* from parser.c: */
extern int v_level;		/* verbosity level (default =0)*/
extern int a_flag;		/* print alternative format (no brackets) */
extern int i_flag;
	/* show items for a sentential form rather than parse sentence */
extern int k;
extern int t;			/* t==-1 means t==inf */

struct rule {
	Symbol LHS;
	Symbol RHS[20];
};
extern int is_Symbol(char c);
extern int is_bracket(char c);

/* from a GRAMMAR.c file */
extern struct rule grammar[];
extern Symbol *input[];
