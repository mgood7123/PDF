#include	"vector.h"	/* for setting MLP bits width */
#include	"grammar.h"
#include	"error.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

char S;				/* start symbol */

vector U;			/* to initiate a parse */
vector V;			/* to test for completion */
vector M[N_TOKENS];		/* Modified Syntax Matrix */
char Precedence[N_TOKENS][N_TOKENS];

typedef struct {
	int number;
	char LHS;
	char *RHS;
} rule;

static rule grammar[] = {	/* ordered as in M[][] */
	{1, 'E', "E+T"},
	{3, 'T', "T*F"},
	{5, 'F', "F^p"},
	{2, 'E', "T"},
	{4, 'T', "F"},
	{6, 'F', "p"},
	{0, 0, 0}
};

int
ProductionNumber(int i) {
	/* production numbers start at 1 */
	return grammar[i-1].number;
}

static int
rule_with_PN(int m) {
	int i;
	
	for (i = 0; grammar[i].LHS != 0; i++) {
		if (grammar[i].number == m) return i;
	}
	abort();
	return -1;
}

char
LHS(int m) {
	return grammar[rule_with_PN(m)].LHS;
}

int
is_nonterminal(int t) {
	int i;
	
	for (i = 0; grammar[i].LHS != 0; i++) {
		if (t == grammar[i].LHS) return 1;
	}
	return 0;
}

const char *
RHS(int m) {
	return grammar[rule_with_PN(m)].RHS;
}

int
LengthRHS(int m) {
	return strlen(grammar[rule_with_PN(m)].RHS);
}

int
SLPwithRHS(char p) {
	int i;
	
	for (i = 0; grammar[i].LHS != 0; i++) {
		if (grammar[i].RHS[0] == p && grammar[i].RHS[1] == 0)
			return grammar[i].number;
	}
	return -1;
}

static int MLP_width = M_WIDTH;

void
NoBitsForSLP(void) {
	M_width = MLP_width;
}

static void
CheckGrammar(void) {
	int i;
	int width;

	/* check M_WIDTH */
	width = 0;
	for (i = 0; grammar[i].LHS != 0; i++) {
		int w = strlen(grammar[i].RHS);
		
		if (w == 1 && MLP_width == M_WIDTH) {
			/* reduce width of MLP section */
			MLP_width = width;
		}
		width += w;
	}
	if (M_WIDTH != width) error("M_WIDTH not OK");
}

static void
Set_U(void) {
	int i;
	int pos;

	pos = 0;
	for (i = 0; grammar[i].LHS != 0; i++) {
		U.bit[pos] = 1;
		pos += strlen(grammar[i].RHS);
	}
}

static void
Set_V(void) {
	int i;
	int pos;

	pos = 0;
	for (i = 0; grammar[i].LHS != 0; i++) {
		pos += strlen(grammar[i].RHS);
		V.bit[pos-1] = 1;
	}
}

static void
Set_M(void) {
	int i;
	int pos;

	pos = 0;
	for (i = 0; grammar[i].LHS != 0; i++) {
		const char *rhs = grammar[i].RHS;
		int j;

		for (j = 0; rhs[j] != 0; j++) {
			M[rhs[j]].bit[pos++] = 1;
		}
	}
}

static void
Set_Precedence(void) {
	/* ad-hoc */
	Precedence['E']['+'] = '=';

	Precedence['T']['+'] = '>';
	Precedence['T']['*'] = '=';
	Precedence['T'][EOI] = '>';

	Precedence['F']['+'] = '>';
	Precedence['F']['*'] = '>';
	Precedence['F']['^'] = '=';
	Precedence['F'][EOI] = '>';

	Precedence['p']['+'] = '>';
	Precedence['p']['*'] = '>';
	Precedence['p']['^'] = '>';
	Precedence['p'][EOI] = '>';

	Precedence['+']['p'] = '<';
	Precedence['*']['p'] = '<';
	Precedence['^']['p'] = '<';
	Precedence[BOI]['p'] = '<';
}

void
InitializeGrammarTables(void) {
	CheckGrammar();

	S = grammar[0].LHS;

	Set_U();
	Set_V();
	Set_M();

	Set_Precedence();
}
