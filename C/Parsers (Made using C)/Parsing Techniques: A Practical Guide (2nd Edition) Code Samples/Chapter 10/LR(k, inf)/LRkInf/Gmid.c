#include	"itemset.h"
#include	"parser.h"

Symbol *input[] = {"aaaaacbccb", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "aSb"},
	{'S', "aSc"},
	{'S', ""},
	{0, ""}
};