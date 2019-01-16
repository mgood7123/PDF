#include	"itemset.h"
#include	"parser.h"

Symbol *input[] = {"vvi", "vvvvvr", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "Ii"},
	{'S', "Rr"},
	{'I', "VI"},
	{'I', "V"},
	{'V', "v"},
	{'R', "WR"},
	{'R', "W"},
	{'W', "v"},
	{0, ""}
};
