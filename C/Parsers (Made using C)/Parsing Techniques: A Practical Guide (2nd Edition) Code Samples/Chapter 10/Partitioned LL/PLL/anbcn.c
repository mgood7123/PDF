/* A PLL(1) parser for
   
   S -> A | B
   A -> aAb | ab
   B -> aBc | ac
*/


#include	"anbcn.h"
#include	"driver.h"

static
char A_or_B(void) {
	/* common prefix */
	token('a');
	/* decision point */
	switch (dot) {
	case 'b': token('b'); print("A->ab"); return 'A';
	case 'c': token('c'); print("B->ac"); return 'B';
	case 'a':
		switch (A_or_B()) {
		case 'A': token('b'); print("A->aAb"); return 'A';
		case 'B': token('c'); print("B->aBc"); return 'B';
		}
	default: return error("abc");
	}
}

char S(void) {
	/* common prefix */
	/* decision point */
	switch (dot) {
	case 'a':
		switch (A_or_B()) {
		case 'A': print("S->A"); return 'S';
		case 'B': print("S->B"); return 'S';
		}
	default: return error("a");
	}
}
