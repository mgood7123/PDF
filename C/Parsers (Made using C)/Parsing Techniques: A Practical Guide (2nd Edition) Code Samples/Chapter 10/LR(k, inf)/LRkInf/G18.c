#include	"itemset.h"
#include	"parser.h"

Symbol *input[] = {"accddb", "bccddb", "aAa", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "aAa"},
	{'S', "bAb"},
	{'S', "aBb"},
	{'S', "bBa"},
	{'A', "cAC"},
	{'A', "cC"},
	{'B', "cBD"},
	{'B', "cD"},
	{'C', "d"},
	{'D', "d"},
	{0, ""}
};
