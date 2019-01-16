#include	"itemset.h"
#include	"parser.h"

Symbol *input[] = {"Aa", "ddcddb", "ddcddddb", 0};

struct rule grammar[] = {
	{'Z', "S"},
	{'S', "Aa"},
	{'S', "Bb"},
	{'A', "dAC"},
	{'A', "c"},
	{'B', "dBD"},
	{'B', "c"},
	{'C', "d"},
	{'D', "dd"},
	{0, ""}
};
