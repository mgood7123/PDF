#include	"itemset.h"
#include	"parser.h"

Symbol *input[] = {"eedf", "eeAeef", "edef", "eedeef", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "AF"},
	{'S', "BG"},
	{'A', "eAe"},
	{'A', "d"},
	{'B', "eBe"},
	{'B', "d"},
	{'F', "f"},
	{'G', "f"},
	{0, ""}
};
