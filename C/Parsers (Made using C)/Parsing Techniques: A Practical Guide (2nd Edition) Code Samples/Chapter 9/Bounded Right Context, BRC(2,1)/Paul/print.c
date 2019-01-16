#include	<stdio.h>
#include	<string.h>

#include	"global.h"
#include	"print.h"

static
print_list(const char list[], const char *name) {
	int i;

	printf("%s: ", name);
	for (i = 0; i < NSYM; i++) {
		if (list[i])
			printf("%c ", i);
	}
	printf("\n");
}

static
print_set(char set[][NSYM], const char *name) {
	int i;

	printf("%s: ", name);
	for (i = 0; i < NSYM; i++) {
		int j;

		for (j = 0; j < NSYM; j++) {
			if (set[i][j])
				printf("(%c,%c) ", i, j);
		}
	}
	printf("\n");
}

void
print_grammar(void) {
	int i;

	for (i = 1; i <= m; i++) {
		printf("%c ::= %c\n", U[i], V[i]);
	}
	for (i = 1; i <= n; i++) {
		printf("%c ::= %c%c\n", R[i], S[i], T[i]);
	}
}

void
print_tables(void) {
	print_list(M, "M");
	print_set(ALPHA, "ALPHA");
	print_set(OMEGA, "OMEGA");
	print_set(MU, "MU");
}

void
print_RTTsize(void) {
	printf("RTT size = %d, %d entries left\n", rsize, RTTSIZE-rsize);
}

void
print_RTT(void) {
	int i;

	for (i = 0; i < rsize; i++) {
		char *rtt = RTT[i];

		printf("%c%c%c -> %d(%c)\n",
		rtt[BELOW], rtt[TOP], rtt[AHEAD], rtt[ACT], rtt[LHS]);
	}
}

void
print_status(const char *X, int r, int s) {
	int isize = strlen(X);
	int i;

	for (i = 0; i < isize - r; i++)
		putchar(' ');
	for (i = 0; i <= r; i++)
		putchar(X[i]);
	putchar(' ');
	for (i = s; i < isize; i++)
		putchar(X[i]);
	for (i = 0; i <= s; i++)
		putchar(' ');
}

