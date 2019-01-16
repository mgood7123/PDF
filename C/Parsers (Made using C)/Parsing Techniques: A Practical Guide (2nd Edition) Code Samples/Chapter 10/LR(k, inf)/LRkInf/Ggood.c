#include	"itemset.h"
#include	"parser.h"

Symbol *input[] = {"aaaabaaaa", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "aSa"},
	{'S', "b"},
	{0, ""}
};
