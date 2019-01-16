/* This is the LR(0,2) grammar of Knuth (1965), pg. 638. Since definitions
   differ, it is not LR(0,inf) or LR(1,inf) but is LR(2,inf).
*/

#include	"itemset.h"
#include	"parser.h"

Symbol *input[] = {"abbbbbbbd", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "ACc"},
	{'S', "BCd"},
	{'A', "a"},
	{'B', "a"},
	{'C', "bC"},
	{'C', "b"},
	{0, ""}
};
