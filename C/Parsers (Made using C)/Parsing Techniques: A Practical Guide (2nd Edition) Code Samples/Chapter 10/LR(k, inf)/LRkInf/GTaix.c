#include	"itemset.h"
#include	"parser.h"

Symbol *input[] = {"vvxi", "vvvvvyyyyr", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "Ii"},
	{'S', "Rr"},
	{'I', "VIx"},
	{'I', "V"},
	{'V', "v"},
	{'R', "WRy"},
	{'R', "W"},
	{'W', "v"},
	{0, ""}
};
