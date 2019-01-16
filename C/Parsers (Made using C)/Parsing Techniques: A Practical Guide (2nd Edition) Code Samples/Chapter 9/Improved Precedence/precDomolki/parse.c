/*	Algorithm W from Shyamasundar's
	Precedence parsing using Domolki's algorithm
	Intern. J. Comput. Math., A6, 1977, pp. 105-114
	
	corrected by Dick Grune, Vrije Universiteit, Amsterdam. (DG)
*/

/*	The published algorithm handles single-length productions
	specially, but there seems to be no reason for this.  The
	algorithm works equally well with SLP defined and undefined:
*/

#undef	SLP	/* modified algorithm without special treatment of SLPs */
#define	SLP	/* original algorithm with special treatment of SLPs */

#include	"vector.h"
#include	"grammar.h"
#include	"report.h"
#include	"stack.h"
#include	"error.h"
#include	"parse.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"pdebug.i"

#define	DEBUG	("a" == "b")	/* construct to suppress lint complaints */

void
Parse(const char *input) {
#define	INPUT(i)	(	i >= strlen(input) \
			?	(error("parser read past EOI"), 0) \
			:	input[i] \
			)

	char y;			/* the symbol currently scanned */
	int i;			/* portion of the input string processed */
	int j;			/* number of symbols on the stack - 1 */
	/* the top item of the stack is addressed as (p, q) */
#define	p	top_of_stack[0].first
#define	q	top_of_stack[0].second

	int m;			/* reduction rule number */
	char A;			/* reduction rule non-terminal */
	int r;			/* reduction rule length */

	vector Q;		/* temporary row vector */

#ifdef	SLP
	NoBitsForSLP();		/* suppress bits for Single Length Prodns */
#endif

Step_1:
				if (DEBUG) printf("Step_1:\n");
/*	i = 1; j = 0; p = '|-'; Q = q = 0. */
	i = 1; j = 0; shift_to_the_stack(BOI, Zero); Q = Zero;

Step_2a:
				if (DEBUG) printf("Step_2a:\n");
/*	y = INPUT(i); Q = (1/2 Q | U) & M[y, *]. */
	y = INPUT(i);

Step_2b:
				if (DEBUG) printf("Step_2b:\n");
/*	if p .> y go to step 3; */
				if (DEBUG) printf("p = %c, y = %c, %c\n", p, y, Precedence[p][y]);
	if (Precedence[p][y] == '>') goto Step_3a;

/*	otherwise shift y to the top of the stack; */

	/* moved; DG */
				if (DEBUG) PV("Step 2a, old Q", Q);
	Q = And(Or(Half(Q), U), Row(M, y));
				if (DEBUG) PV("Step 2a, new Q", Q);

	shift_to_the_stack(y, Q);
				if (DEBUG) pr_stack("after shift y", stack, j + 1);

/*	j = j + 1; i = i + 1; go to step 2(a); */
	j = j + 1; i = i + 1; goto Step_2a;

Step_3a:
				if (DEBUG) printf("Step_3a:\n");
/*	if Q ^ V = 0 go to step 5; */
#ifdef	SLP
	if (Eq(And(Q, V), Zero)) goto Step_5;
#endif
				if (DEBUG) PV("3a:", Q);

/*	otherwise ... */
	r = First1BitPos(And(Q, V));		/* was Q; DG */
	m = ProductionNumber(Number1Bits(V, r + 1));
	A = LHS(m);
	r = LengthRHS(m);
				if (DEBUG) printf("m = %d, A = %c, r = %d\n", m, A, r);

Step_3b:
				if (DEBUG) printf("Step_3b:\n");
/*	Remove r records from the stack, */
	remove_from_the_stack(r);

/*	and output the corresponding production number; */
	AddProductionToParse(m);

/*	Set j = j - r - 1; */
	j = j - r + 1;				/* modf. DG */

/*	Q = (1/2 q | U) & M[A, *]; */
				if (DEBUG) PV("Step 3b, old q", q);
	Q = And(Or(Half(q), U), Row(M, A));
				if (DEBUG) PV("Step 3b, new Q", Q);

/*	shift the symbol A to the stack; */
	shift_to_the_stack(A, Q);
				if (DEBUG) pr_stack("after shift A", stack, j);

/*	Set p = A; */
	;			/* useless; DG */

/*	if p = S, j = 1 and input sequence is exhausted, go to step 6; */
				if (DEBUG) printf("p = %c, j = %d, INPUT(i) = %c\n", p, j, INPUT(i));
	if (p == S && j == 1 && INPUT(i) == EOI) goto Step_6;

/*	otherwise go to step 2(b). */
	goto Step_2b;

Step_4:
				if (DEBUG) printf("Step_4:\n");
/*	Error found. */
	printf("Error found.\n"); exit(1);


#ifdef	SLP
Step_5:
				if (DEBUG) printf("Step_5:\n");
/*	If there is no SLP with its right hand side equal to p, go to step 4; */
	m = SLPwithRHS(p); if (m < 0) goto Step_4;

/*	otherwise let the production rule determined be m: A -> p; */
	A = LHS(m);
				if (DEBUG) printf("Step 5: resulting m = %d, A = %c\n", m, A);

/*	Set r = 1; go to step 3(b). */
	r = 1; goto Step_3b;
#endif

Step_6:
				if (DEBUG) printf("Step_6:\n");
/*	Output the parse. */
	return;
}
