#include	"itemset.h"
#include	"parser.h"

Symbol *input[] = {"aaaa", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "aSa"},
	{'S', "aa"},
	{0, ""}
};
