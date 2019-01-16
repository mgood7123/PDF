/*	This is a C version of the parsing algorithm given by E.T. Irons,
	"A syntax-directed compiler for ALGOL 60", Commun. ACM, Vol 4, #1, Jan
	1961, pp. 51-55.
	The present version has been modernized a bit.
*/

#include	"global.h"
#include	"driver.h"

static int j = -1;			/* INPUT pointer */
static int k = 1;			/* OUTPUT pointer */

static int FOUND(int SYNU, int GOAL, int *PARAM, int SYNU_I);

int
DIAGRAM(int I, int GOAL, int *PARAM) {
/*	I is the starting position in the syntax table STAB. GOAL is the
	requested syntax unit. If this unit is discovered the index of the
	appropriate definition string is placed in the output string, and the
	*negative* index of the output string is assigned to PARAM. If not the
	procedure returns with the value 0.
*/
	int J, K, i;

	J = j; K = k;
	for (i = I; i != 0; i = STC[i]) {
		if (STAB[i+1] != LEFTBRACE) {
			int OTCEL;

			j++;
			if (	FOUND(INPUT[j], STAB[i], &OTCEL, 0)
			&&	DIAGRAM(i+1, GOAL, PARAM)
			) {
				OUTPUT[k++] = OTCEL;
				return 1;
			}
			j = J; k = K;
		}
	}

	for (i = I; i != 0; i = STC[i]) {
		if (STAB[i+1] == LEFTBRACE) {
			if (FOUND(STAB[i], GOAL, PARAM, -k)) {
				OUTPUT[k++] = i+2;
				return 1;
			}
			j = J; k = K;
		}
	}

	return 0;
}

static int
FOUND(int SYNU, int GOAL, int *PARAM, int SYNU_I) {
/*	The function returns 1 if, having just recognized a SYNU, the syntax
	unit GOAL can be found. The negative index of the output string is
	assigned to PARAM. The index of the output string corresponding to
	SYNU should be given in SYNU_I.
*/

	if (SUCCR[SYNU][GOAL] && DIAGRAM(TRAN[SYNU], GOAL, PARAM)) {
		/* extended GOAL recognized, PARAM set */
		return 1;
	}

	if (SYNU == GOAL) {
		/* single GOAL recognized; set PARAM */
		*PARAM = SYNU_I;
		return 1;
	}

	/* nothing recognized */
	return 0;
}

