/* computes V[k,inf] as described in Procedure 2.1 on page 67 of Szymanski */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"itemset.h"
#include	"parser.h"
#include	"computeVk.h"

/* Procedure 2.1 has an initialization problem: it creates an item set
   by computing the previous item set recursively and then shifts over
   the next symbol.  But since there is no previous item set for S0,
   the initial state, and no shift is involved in its creation, the
   procedure cannot be used to create the initial state.

   Szymanski "solves" this by creating a pre-zero item <^(k+1)S]0>^k.
   Next, the shift (Step 2) shifts over the first (spurious) < in the
   item, creating the correct active item <^kS]0>^k.  Step 3 then
   predicts the items of the initial state from it.

   The shift over the spurious < only works because the input has been
   padded with <^k, and since k>0, there is at least one < token.
   This is a kludge, and it is more fair to create the active set V''
   of V[k,inf](X[1]..Y[k]) as follows:

     if n=0 then set V'' = {<^kS]0>^k} and set n=1
     otherwise set V' = V[k,inf](X[1]..Y[k-1]) and obtain V'' by shifting
     as in step 2

*/

static int
pos_of_sym(const Symbol *s, int n) {
	/* return the position of the n-th Symbol in s */
	int i;

	for (i = 0; s[i]; i++) {
		if (is_Symbol(s[i])) {
			n--;
			if (n == 0) return i;
		}
	}
	return -1;			/* not found */
}

static Symbol
sym(const Symbol *s, int n) {
	/* return the n-th Symbol in s */
	int i = pos_of_sym(s, n);

	if (i < 0 || i > strlen(s)) abort();
	return s[i];
}

void
Compute_VkInf(const struct rule *grammar, const Symbol *XY, ItemSet VkInf) {
	static int recursive; /* for avoiding printing from recursive calls */
	int n = strlen(XY) - k;
	const Symbol *Y = &XY[n];
	ItemSet V2;
	ItemSet V3;
	int i;

	/* active set V2 */
	if (n == 0) {
		/* [create] */
		Item I;
		int j;

		clear_Item(I);
		for (j = 0; j < k; j++) {
			add_Symbol(I, '<');
		}
		add_SymbolSeq(I, grammar[0].RHS);
		add_SymbolSeq(I, "]0");
		for (j = 0; j < k; j++) {
			add_Symbol(I, '>');
		}
		clear_ItemSet(V2);
		add_Item(V2, I);
		if (!recursive && v_level > 0) {
			/* fake a report message */
			ItemSet V1;

			clear_ItemSet(V1);
			print_ItemSet("V' (initial)", XY, V1);
		}
	}
	else {
		ItemSet V1;

		/* [recurse] compute VkInf for XY with one symbol less */
		{	SymbolSeq XY1;

			new_SymbolSeq_N(XY1, XY, strlen(XY)-1);
			recursive++;
			Compute_VkInf(grammar, XY1, V1);
			recursive--;
			if (!recursive && v_level > 0)
				print_ItemSet("V' (initial)", XY, V1);
		}

		/* [shift] over X_n */
		clear_ItemSet(V2);
		for (i = 0; is_Item(V1[i]); i++) {
			Symbol X_n = XY[n-1];
			int j;
			
			/* find first symbol in V1[i]; must be X_n */
			j = pos_of_sym(V1[i], 1);
			if (V1[i][j] == X_n) {
				/* add a shifted copy to V2 */
				Item I;
				
				clear_Item(I);
				add_SymbolSeq(I, &V1[i][j+1]);
				add_Item(V2, I);
			}
			else {
				/* discard item if first symbol != X_n */
				/* this can happen when k == 0 */
			}
		}
	}
	if (!recursive && v_level > 0) print_ItemSet("V'' (shifted)", XY, V2);

	/* [close] */
	clear_ItemSet(V3);
	/* add in V2 */
	for (i = 0; is_Item(V2[i]); i++) {
		add_Item(V3, V2[i]);
	}
	/* and perform the closure */
	for (i = 0; is_Item(V3[i]); i++) {
		/* items may be added to V3 by this loop body */
		int pos_Vk;

		/* find the symbol to expand for prediction */
		/* Normally this is the k-th symbol, the last symbol of the
		   look-ahead.  This is because the algorithm combines the
		   prediction and the look-ahead computation.  This k-th
		   symbol is always there, since the input is extended with k
		   closers '>'.
		   For k=0 there is no look-ahead computation but the
		   prediction still has to be done.  But now the symbol to be
		   expanded will not be available at the end of the input.
		   We can either 1. extend the input with an extra '>', which
		   is messy, or 2. expand only when there is a symbol, which
		   is more logical.  So option 2 it is.
		*/
		pos_Vk = pos_of_sym(V3[i], (k != 0 ? k : 1));
		if (!recursive && v_level > 1) printf("pos_Vk = %d\n", pos_Vk);

		if (pos_Vk >= 0) {
			Item m_gamma;
			int j;

			/* construct m(gamma) */
			clear_Item(m_gamma);
			for (j = pos_Vk+1; V3[i][j]; j++) {
				if (is_Symbol(V3[i][j])) {
					add_Symbol(m_gamma, V3[i][j]);
				}
			}
			if (!recursive && v_level > 1)
				printf("m_gamma = `%s'\n", m_gamma);
			
			/* expand Vk */
			for (j = 1; grammar[j].LHS; j++) {
				if (grammar[j].LHS == V3[i][pos_Vk]) {
					Item new;
					Symbol bracket[10];
					
					new_SymbolSeq_N(new, V3[i], pos_Vk);
					add_SymbolSeq(new, grammar[j].RHS);
					sprintf(bracket, "]%d", j);
					add_SymbolSeq(new, bracket);
					add_SymbolSeq(new, m_gamma);
					if (!recursive && v_level > 1)
						printf("`%s' added to V3\n", new);
					
					add_Item(V3, new);
				}
			}
		}
	}
	if (!recursive && v_level > 0) print_ItemSet("V''' (closed)", XY, V3);

	/* [intersect] */
	clear_ItemSet(VkInf);
	for (i = 0; is_Item(V3[i]); i++) {
		int j;
		int mismatch_found = 0;

		/* check if the first k symbols of the item match Y[] */
		for (j = 1; j <= k; j++) {
			if (sym(V3[i], j) != sym(Y, j)) {
				mismatch_found = 1;
			}
		}
		if (!mismatch_found) {
			add_Item(VkInf, V3[i]);
		}
	}
	if (!recursive && v_level > 0)
		print_ItemSet("V[k,inf] (intersected)", XY, VkInf);
}
