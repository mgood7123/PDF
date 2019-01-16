#define	N_TOKENS	128	/* required for Precedence[][] */

extern vector U;		/* to initiate a parse */
extern vector V;		/* to test for completion */
extern vector M[];		/* Modified Syntax Matrix */

extern char S;			/* start symbol */
extern char Precedence[N_TOKENS][N_TOKENS];

#define	BOI	'@'		/* Beginning Of Input */
#define	EOI	'$'		/* End Of Input */

extern void InitializeGrammarTables(void);
extern int ProductionNumber(int i);
extern int is_nonterminal(int t);
extern char LHS(int m);
extern const char *RHS(int m);
extern int LengthRHS(int m);
extern int SLPwithRHS(char p);
extern void NoBitsForSLP(void);
