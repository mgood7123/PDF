/* at present none of the limits below are checked ???? */
#define	MAX_d	100			/* max. number of rules */
#define	MAX_j	10			/* max. length of rhs */
#define	MAX_n	100			/* max. length of input */
#define	MAX_s	(MAX_d * MAX_j)		/* max. number of states per position*/

extern char input[];
extern char start;
extern void grammar(void);
extern void rule(char lhs, const char *rhs);

