#include	<stdio.h>
#include	<stdlib.h>

#include	"dot.h"

extern void LC(void);

static int err_cnt;

/* REPORTING */

static const char *stack[1000];
static int stp = 0;
static int depth = 0;

static void
push(const char *s) {
	if (!s) return;
	stack[stp++] = s;
}
	
static void
pop(void) {
	if (stp <= 0) {fprintf(stderr, "stack underflow\n"); exit(1);}
	stp--;
}

static void
print_stack(const char *msg) {
	int i;

	printf("%s: ", msg);
	for (i = stp; i > 0; i--) {
		printf("%s ", stack[i-1]);
	}
	printf("[%d]\n", stp);fflush(stdout);
}

static const char *
c(const char *s) {
	return s ? s : "";
}

void
start(
	const char *s,
	const char *s1,
	const char *s2,
	const char *s3,
	const char *s4
) {
	/* predict */
	depth++;
	printf("predicted: %s -> %s %s %s %s [%d]\n", s,
	       c(s1), c(s2), c(s3), c(s4), depth
	       );
	pop();
	push(s4); push(s3); push(s2); push(s1);
	print_stack("start");
}

void
stop(const char *s) {
	printf("recognized nt: %s [%d]\n", s, depth);
	depth--;
}

void
LC_done(char *nt) {
	printf("recognized LC: %s\n", nt);
	print_stack("LC done");
}

void
term_done(char *nt) {
	printf("recognized term: %s [%d]\n", nt, depth);
	pop();
	print_stack("term done");
}

void
nt_done(void) {
	print_stack("nt done");
}

/* INPUT */

static int DOT;

int
yylex(void) {
	DOT = getchar();

	return DOT == '\n' ? 256 : DOT;
}

void
LLmessage(int tk) {
	err_cnt++;
	if (tk < 0) {
		printf("DOT = %c, end of file expected\n", DOT);
	}
	else if (tk) {
		printf("%c missing\n", tk);
	}
	else {
		printf("%c deleted\n", DOT);
	}
}

int
main(void) {
	err_cnt = 0;

	/* patch: where can one put this code?? */
	push("#");
	push("S");
	print_stack("initial");
	printf("depth = %d\n", depth);
	LC();
	printf("depth = %d\n", depth);
	stop("#");

	return (err_cnt == 0 ? 0 : 1);
}
