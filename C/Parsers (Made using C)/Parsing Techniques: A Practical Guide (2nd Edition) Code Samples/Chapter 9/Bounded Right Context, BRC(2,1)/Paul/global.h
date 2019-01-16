#define	GRSIZE	100			/* grammar size */
#define	RTTSIZE	1000			/* reducing transition table size */
#define	XSIZE	100			/* internal buffer size */
#define	NSYM	128

extern int m;				/* # of productions of length 1 */
extern int n;				/* # of productions of length 2 */
extern char U[GRSIZE], V[GRSIZE];	/* productions of length 1 */
extern char R[GRSIZE], S[GRSIZE], T[GRSIZE];
					/* productions of length 2 */

extern char M[NSYM];			/* set for all syntactic symbols */
extern char ALPHA[NSYM][NSYM];
extern char OMEGA[NSYM][NSYM];
extern char MU[NSYM][NSYM];

extern int rsize;			/* size of reducing trans. table */
extern char RTT[RTTSIZE][5];		/* reducing transition table */
#define	BELOW	0
#define	TOP	1
#define	AHEAD	2
#define	ACT	3
#define	LHS	4

extern char input[];
extern char start;
extern void grammar(void);
extern void rule(char lhs, const char *rhs);
