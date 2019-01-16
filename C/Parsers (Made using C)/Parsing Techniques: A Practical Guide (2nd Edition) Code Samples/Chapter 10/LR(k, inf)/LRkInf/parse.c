#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"itemset.h"
#include	"parser.h"
#include	"computeVk.h"
#include	"parse.h"

static void
pack_with_markers(Symbol *to, const Symbol *from, int left, int right) {
	/* to := '<'^left from '>'^right */
	int i;

	to[0] = '\0';
	/* prepend <^left */
	for (i = 0; i < left; i++) {
		add_Symbol(to, '<');
	}
	add_SymbolSeq(to, from);
	/* append >^right */
	for (i = 0; i < right; i++) {
		add_Symbol(to, '>');
	}
}

/* TEST_SENTENTIAL_FORM */
void
test_sentential_form(const Symbol *s_form) {
	/* print item sets for increasing prefixes of s_form */
	int sf_length = strlen(s_form);
	int i;

	for (i = 0; i <= sf_length; i++) {
		SymbolSeq X;
		ItemSet VkInf;

		pack_with_markers(X, s_form, k, 0);
		X[k+i] = '\0';		/* clip X[] */
		Compute_VkInf(grammar, X, VkInf);
		print_ItemSet("VkInf", X, VkInf);
	}
}

/* PARSE_SENTENCE */
static int
phrase_found(ItemSet V) {
	/* returns production number of left-most bracket if all items
	   agree on it; 0 otherwise
	*/
	int j;			/* i already used by Szymanski */
	int i = 0;		/* 0 is not a production number */
	
	for (j = 0; is_Item(V[j]); j++) {
		int prod_nmb;

		if  (!is_bracket(V[j][0])) return 0;
		prod_nmb = atoi(&V[j][1]);
		if (i == 0) {
			i = prod_nmb;
		}
		else {
			if (prod_nmb != i) return 0;
		}
	}
	
	return i;
}

static void
reduce(Symbol *X, int n, int i) {
	/* reduce the segment just in front of X[n] by grammar rule i */
	Symbol LHS = grammar[i].LHS;
	Symbol *RHS = grammar[i].RHS;
	int RHS_length = strlen(RHS);
	SymbolSeq X1;

	/* check if the RHS is indeed in place */
	if (strncmp(&X[n-RHS_length], RHS, RHS_length) != 0) {
		fprintf(stderr, "Reduce error: X =%s, RHS = %s, n = %d\n",
			X, RHS, n
		);
		exit(1);
	}

	/* reduce; play it safe, we may be reducing epsilon */
	new_SymbolSeq_N(X1, X, n-RHS_length);
	add_Symbol(X1, LHS);
	add_SymbolSeq(X1, &X[n]);
	printf("Reduce %s to %s by production %d (%c->%s) at pos %d\n",
		X, X1, i, LHS, RHS, n
	);

	clear_SymbolSeq(X);
	add_SymbolSeq(X, X1);
}

static int
contains_brackets(ItemSet V) {
	/* return 1 if the item contains at least one bracket; 0 otherwise */
	int i;
	
	for (i = 0; is_Item(V[i]); i++) {
		int j;

		for (j = 0; V[i][j]; j++) {
			if (!is_Symbol(V[i][j])) return 1;
		}
	}
	return 0;
}

void
parse_sentence(const Symbol *s_form) {
	/* Algorithm 2.3 from page 72 of Szymanski's thesis */
	SymbolSeq X;
	SymbolSeq Z;

	pack_with_markers(X, s_form, k, k);
	pack_with_markers(Z, grammar[0].RHS, k, k);

	while (strcmp(X, Z) != 0) {
		/* find a phrase in X */
		int n;
		int m = strlen(X);
		ItemSet VkInf;

Step1:		n = 0;

Step2:		{	SymbolSeq X1;

			new_SymbolSeq_N(X1, X, n+k);
			Compute_VkInf(grammar, X1, VkInf);
			if (v_level > 1) {
				printf("at n = %d, ", n);
				print_ItemSet("VkInf", X1, VkInf);
			}
		}

Step3:		if (is_empty_ItemSet(VkInf)) {
			printf("The string is not a sentence\n");
			return;
		}

Step4:		
		if (phrase_found(VkInf)) {
			reduce(X, n, phrase_found(VkInf));
			continue;
		}

Step5:		if (!contains_brackets(VkInf)) {
			printf("The grammar is not LR(%d,inf)\n", k);
			return;
		}

Step6:		if (n+k == m) {
			printf("The grammar is ambiguous and not LR(%d,inf)\n",
			       k);
			return;
		}

Step7:		n = n+1;
		goto Step2;
	}
	printf("Parsing done\n");
}
