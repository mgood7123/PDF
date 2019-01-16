#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"itemset.h"
#include	"parser.h"

/* Since we want to be able to represent empty items in the item sets, we need
   an explicit terminator to the list of items.
*/

#define	NO_ITEM		'\255'

void
clear_Item(Item I) {
	I[0] = '\0';
}

int
is_Item(const Item I) {
	return I[0] != NO_ITEM;
}

int
is_empty_ItemSet(ItemSet V) {
	return !is_Item(V[0]);
}

void
clear_SymbolSeq(Symbol *s) {
	s[0] = '\0';
}

static void
check_overflow(int size) {
	if (size > MAX_STRINGSIZE) {
		fprintf(stderr,
			"Overflow; max. string, item and set size = %d\n",
			MAX_STRINGSIZE
		);
		exit(1);
	}
}

void
new_SymbolSeq(Symbol *to, const Symbol *from) {
	check_overflow(strlen(from)+1);
	strcpy(to, from);
}

void
new_SymbolSeq_N(Symbol *to, const Symbol *from, int size) {
	check_overflow(size+1);
	strncpy(to, from, size);
	to[size] = '\0';
}

void
add_SymbolSeq(Symbol *to, const Symbol *from) {
	check_overflow(strlen(to) + strlen(from) + 1);
	strcat(to, from);
}

void
add_Symbol(Symbol *to, Symbol ch) {
	int p = strlen(to);

	check_overflow(p+2);
	to[p] = ch;
	to[p+1] = '\0';
}

void
clear_ItemSet(ItemSet V) {
	V[0][0] = NO_ITEM;
}

void
add_Item(ItemSet V, const Item I) {
	int i;

	/* check for duplicates */
	for (i = 0; is_Item(V[i]); i++) {
		if (strcmp(I, V[i]) == 0) return;
	}
	check_overflow(i+1);
	strcpy(V[i], I);
	V[i+1][0] = NO_ITEM;
}

static void
print_Item_alt(const Symbol *X, Item I) {
	/* print actual items rather than brackets */
	int len_X = strlen(X);
	int b = 0;	/* tracks position of bracket in I */
	int is_first_item = 1;
	
	/* Szymansky shows the part after the dot only, but the classical
	   item has the last p-q tokens of the input before the dot, where
	   p is the length of the hypothesized right-hand side and q is the
	   length of the unmatched part of it.
	*/

	while (I[b] != '\0') {
		/* b starts one position after the latest bracket */
		Symbol LHS;	/* LHS of rule of bracket */
		int p;		/* length of RHS of rule of bracket */
		int q;		/* length of unmatched part of RHS */
		int r = b; /* position in I of first symbol not yet printed */

		/* find the next bracket, if present */
		while (I[b] != '\0' && !is_bracket(I[b])) {
			b++;
		}

		/* get the attributes of its production rule */
		if (I[b] != '\0') {
			/* we have a bracket */
			int prod_nmb = atoi(&I[b+1]);
			Symbol *RHS = grammar[prod_nmb].RHS;
			
			LHS = grammar[prod_nmb].LHS;
			/* length of the RHS */
			p = strlen(RHS);
			/* length of the unmatched part of it: */
			q = (r == 0 ? b : p);	/* = b for the first item */
			q = (q > p ? p : q);	/* with a maximum of p */
		}
		else {	/* no bracket left */
			LHS = '\0';
			p = 0;
			q = 0;
		}

		/* print the non-expanded symbols between I[r] and I[b-p-1] */
		while (r < b-p) {
			printf("%c", I[r]);
			r++;
		}
		if (I[b] == '\0') break;

		/* print the item; special for first item */
		printf("%s%c->", (is_first_item ? "" : "["), LHS);
		printf("%s", &X[len_X-(p-q)]);	/* empty unless first item */
		printf(".");
		while (q > 0) {
			printf("%c", I[b-q]);
			q--;
		}
		printf("%s", (is_first_item ? " " : "]"));

		/* skip the bracket */
		while (I[b] != '\0' && !is_Symbol(I[b])) {
			b++;
		}
		/* b is now one position after the latest bracket or at end */
		is_first_item = 0;
	}

	printf("\n");
}

void
print_ItemSet(const char *name, const Symbol *XY, ItemSet V) {
	SymbolSeq X;			/* input before the dot */
	SymbolSeq Y;			/* input after the dot */
	int item_count = 0;
	int i;

	/* split XY into X and Y */
	new_SymbolSeq_N(X, XY, strlen(XY)-k);
	new_SymbolSeq(Y, &XY[strlen(XY)-k]);

	/* count the items */
	for (i = 0; is_Item(V[i]); i++) {
		item_count++;
	}

	/* report */
	printf("ItemSet %s for %s.%s (%d item%s):\n",
	       name, X, Y, item_count, (item_count == 1 ? "" : "s")
	);

	for (i = 0; is_Item(V[i]); i++) {
		if (a_flag) {
			print_Item_alt(X, V[i]);
		}
		else {
			printf("%s\n", V[i]);
		}
	}
	printf("\n");
}
