/*	This is a C version of the parsing algorithm given by E.T. Irons,
	"A syntax-directed compiler for ALGOL 60", Commun. ACM, Vol 4, #1, Jan
	1961, pp. 51-55.
	It is kept as close as possible to the ALGOL 60 original.
*/

#include	"global.h"
#include	"driver.h"

static int j = -1;			/* INPUT pointer */
static int k = 1;			/* OUTPUT pointer */

int
DIAGRAM(int i, int GOAL, int *PARAM)
/*	i is the starting position in the syntax table STAB. GOAL is the
	requested syntax unit. If this unit is discovered the index of the
	appropriate definition string is placed in the output string, and the
	*negative* index of the output string is assigned to PARAM. If not the
	procedure returns with the value 0.
*/
{
	int J, K, I, OTCEL = -1;
	int SW;

	if (i == 0) return 0;

	J = j; K = k; I = i;
START:	if (STAB[i+1] != LEFTBRACE) {
		j = j + 1;
		SW = (INPUT[j] == STAB[i] ? 1 : 0);
		if (SUCCR[INPUT[j]][STAB[i]]) {
			if (!DIAGRAM(TRAN[INPUT[j]], STAB[i], &OTCEL))
				goto NOGO;
			goto CONTINUE;
		}
		else {
		NOGO:	if (SW) goto CONTINUE;
			else {
			RETRACE:j = J; k = K;
			}
		}
	}
	i = STC[i];
	if (i != 0) goto START;
	i = I;
NEWSTART:
	if (STAB[i+1] == LEFTBRACE) {
		OTCEL = i+2;
		if (STAB[i] == GOAL) {*PARAM = -k; SW = 1;}
		else	SW = 0;
		if (SUCCR[STAB[i]][GOAL]) {
			if (!DIAGRAM(TRAN[STAB[i]], GOAL, PARAM))
				goto NOPATH;
			goto FOUND;
		}
		else {
		NOPATH:	if (SW) goto FOUND;
		}
	}
	i = STC[i];
	if (i != 0) goto NEWSTART;
	j = J;
	k = K;
	return 0;
CONTINUE:
	if (!DIAGRAM(i+1, GOAL, PARAM)) goto RETRACE;
FOUND:	OUTPUT[k] = OTCEL;
	k = k + 1;
	return 1;
}
