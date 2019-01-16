#include	"table.h"

const char *initial_state = "Z[Start]";
const struct transition transition[] = {
	/* This is the 2-way PDA described by Cook in his Information
	   Processing 71 paper.  Unfortunately the original version
	   accepts $wu$ with $w = w^R$ with $w^R$ the reverse of $w$
	   rather than $ww^Ru$.  And since every string starts with at
	   least a 1-character palindrome, it accpets all strings.

	   The corrected version distinguishes between even and odd
	   tokens. It now accepts exactly $ww^Ru$.
	*/
#undef	WRONG_VERSION

	/* Copy to stack */
	{"Z[Start]", "<",	"Z[Copy]", +1,	"start"},
	{"[Copy]", "a",		"a[Copy]", +1,	"a copied"},
	{"[Copy]", "b",		"b[Copy]", +1,	"b copied"},
	{"[Copy]", ">",		"[Back]", -1,	"switch to reverse"},

	/* Reverse head to beginning */
	{"[Back]", "a",		"[Back]", -1,	"reverse"},
	{"[Back]", "b",		"[Back]", -1,	"reverse"},
	{"[Back]", "<",		"[Comp]", +1,	"switch to compare"},

	/* Restore */
	{"[Rest]", "a",		"a[Rest]", -1,	"a restored"},
	{"[Rest]", "b",		"b[Rest]", -1,	"b restored"},
	{"a[Rest]", "<",	"[Comp]", +1,	"pop a and switch to compare"},
	{"b[Rest]", "<",	"[Comp]", +1,	"pop b and switch to compare"},

	/* Compare */
#ifdef	WRONG_VERSION
	{"a[Comp]", "a",	"[Comp]", +1,	"accept a"},
	{"b[Comp]", "b",	"[Comp]", +1,	"accept b"},
	{"a[Comp]", "b",	"a[Rest]", -1,	"reject b"},
	{"b[Comp]", "a",	"b[Rest]", -1,	"reject a"},
	/* and switch to Restore */
#else
	{"a[Comp]", "a",	"[Cmp1]", +1,	"accept even a"},
	{"b[Comp]", "b",	"[Cmp1]", +1,	"accept even b"},
	{"a[Cmp1]", "a",	"[Comp]", +1,	"accept odd a"},
	{"b[Cmp1]", "b",	"[Comp]", +1,	"accept odd b"},
	{"a[Comp]", "b",	"a[Rest]", -1,	"reject even b"},
	{"b[Comp]", "a",	"b[Rest]", -1,	"reject even a"},
	{"a[Cmp1]", "b",	"a[Rest]", -1,	"reject odd b"},
	{"b[Cmp1]", "a",	"b[Rest]", -1,	"reject odd a"},
	/* and switch to Restore */
#endif
	/* or accept */
	{"Z[Comp]", "a",	0, 0,		"accept input"},
	{"Z[Comp]", "b",	0, 0,		"accept input"},

	{0}
};
