#include	"itemset.h"
#include	"parser.h"

/* page 54 */
Symbol *input[] = {"ccccdcccceea", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "AF"},
	{'S', "BG"},
	{'A', "cAC"},
	{'A', "d"},
	{'B', "cBD"},
	{'B', "d"},
	{'C', "c"},
	{'D', "c"},
	{'F', "eF"},
	{'F', "a"},
	{'G', "eG"},
	{'G', "b"},
	{0, ""}
};
