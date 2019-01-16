/*	Algorithm W from Shyamasundar's
	Precedence parsing using Domolki's algorithm
	Intern. J. Comput. Math., A6, 1977, pp. 105-114
	
	rewritten by Dick Grune, Vrije Universiteit, Amsterdam.
*/

#include	"vector.h"
#include	"grammar.h"
#include	"stack.h"
#include	"report.h"
#include	"error.h"
#include	"parse.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"pdebug.i"

static vector
Domolki(vector q, int y) {
	return And(Or(Half(q), U), Row(M, y));
}

void
Parse(const char *input) {
	int i;			/* portion of the input string processed */
#define	INPUT(i)	(	i >= strlen(input) \
			?	error("parser read past EOI") \
			:	input[i] \
			)

	char look_ahead;

	/* initiate stack */
	shift_to_the_stack(BOI, Zero);

	/* initiate input, skipping the BOI symbol */
	i = 1; look_ahead = INPUT(i);

	while (!(	top_of_stack[-1].first == BOI
		&&	top_of_stack[0].first == S
		&&	look_ahead == EOI
		)
	) {
		char z;		/* terminal or nonterminal to be stacked */

		/* perform the ACTION */
		switch (Precedence[top_of_stack[0].first][look_ahead]) {
			int m;	/* reduction rule number */

		case '<': case '=':
			/* shift */
			/* initiate stacking the input symbol */
			z = look_ahead;

			/* get a new look-ahead */
			i = i + 1; look_ahead = INPUT(i);
			break;

		case '>':
			/* reduce */
			m = ProductionNumber(
				Number1Bits(
					V,
					First1BitPos(
						And(top_of_stack[0].second, V)
					) + 1
				)
			);

			AddProductionToParse(m);
			remove_from_the_stack(LengthRHS(m));

			/* initiate stacking the result of the reduction */
			z = LHS(m);
			break;

		default:
			z = 0;
			printf("Error found in the input.\n"); exit(1);
		}

		/* perform the GOTO */
		shift_to_the_stack(z, Domolki(top_of_stack[0].second, z));
	}
}
