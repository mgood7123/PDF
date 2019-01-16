#include	"itemset.h"
#include	"parser.h"

Symbol *input[] = {"(((", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "E$"},
	{'E', "E-T"},
	{'E', "T"},
	{'T', "n"},
	{'T', "(E)"},
	{0, ""}
};
