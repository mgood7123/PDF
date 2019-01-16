#include	<stdio.h>

#include	"global.h"
#include	"grammar.h"

char N[MAX_d];
int n_N;

char D[MAX_d];
char C[MAX_d][MAX_j];
int bar[MAX_d];	
int d;		

const char D0 = '*';
const char end = '#';

void
print_grammar(void) {
	int p;

	printf("rules: d = %d\n", d);
	for (p = 0; p <= d; p++) {
		printf("%c -> %s, bar = %d\n", D[p], &C[p][1], bar[p]);
	}

	printf("N = {");
	for (p = 0; p < n_N; p++) {
		printf(" %c", N[p]);
	}
	printf("}\n");
}

void
rule(char lhs, const char *rhs) {
	int j;

	/* collect the rule */
	d++;
	D[d] = lhs;
	for (j = 0; rhs[j]; j++) {
		C[d][j+1] = rhs[j];
	}
	C[d][j+1] = '\0';
	bar[d] = j;

	/* put its lhs in N[] is not there already */
	for (j = 0; j < n_N; j++) {
		if (N[j] == D[d]) return;
	}
	N[n_N++] = D[d];
}
