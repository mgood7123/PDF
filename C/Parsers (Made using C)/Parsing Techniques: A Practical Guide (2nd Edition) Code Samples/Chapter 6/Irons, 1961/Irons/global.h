#define	NSYNU		128
#define	GRAMSIZE	10000

#define	LEFTBRACE	'{'
#define	RIGHTBRACE	'}'
#define	DUMMYSTART	'!'		/* the dummy start symbol, */
					/* see comment in init_grammar() */

extern int TRAN[NSYNU];			/* from syntax unit to syntax table */
extern int STAB[GRAMSIZE];		/* the rules in the syntax table */
extern int STC[GRAMSIZE];		/* the links to further rules */
extern int SUCCR[NSYNU][NSYNU];		/* the FIRST relation */

extern char *INPUT;
extern int OUTPUT[];

extern char input[];
extern char start;

