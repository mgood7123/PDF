/*	The example from
	J. Eickel, M. Paul, F.L. Bauer, K. Samelson,
	A syntax-controlled generator of formal language processors,
	Commun. ACM, Vol. 6, no. 8, Aug 1963, 451-455.
*/

#include	"global.h"

char input[] = "adbecadbece";

char start = 'Z';

void
grammar(void) {
	rule('W', "V");
	rule('Z', "e");
	rule('Z', "W");
	rule('U', "Pb");		/* P for U1 */
	rule('P', "ad");
	rule('V', "Ue");
	rule('W', "QZ");		/* Q for U2 */
	rule('Q', "Vc");
}
