#define	SHIFT		'\0'
#define	ACCEPT		'\1'
#define	SERROR		'\2'		/* syntax error */
#define	IERROR		'\3'		/* internal error */

struct entry {
	char l2, l1, r;			/* l2, l1 on stack, r in input */
	char lhs;
	char *rhs;
};

extern const struct entry table[];
extern int tabsize;

