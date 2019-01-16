#include	"vector.h"
#include	"grammar.h"

#include	<stdio.h>
#include	<stdlib.h>

const vector Zero;

int M_width = M_WIDTH;			/* the actual bit width used */

vector
And(const vector a, const vector b) {
	vector res;
	int i;
	
	for (i = 0; i < M_width; i++) {
		res.bit[i] = a.bit[i] & b.bit[i];
	}
	return res;
}

vector
Or(const vector a, const vector b) {
	vector res;
	int i;
	
	for (i = 0; i < M_width; i++) {
		res.bit[i] = a.bit[i] | b.bit[i];
	}
	return res;
}

int
Eq(const vector a, const vector b) {
	int i;
	
	for (i = 0; i < M_width; i++) {
		if (a.bit[i] != b.bit[i]) return 0;
	}
	return 1;
}

vector
Half(const vector a) {
	vector res;
	int i;
	
	for (i = 0; i < M_width; i++) {
		res.bit[i] = (i < 1 ? 0 : a.bit[i-1]);
	}
	return res;
}

vector
Row(const vector a[], int i) {
	return a[i];
}

int
First1BitPos(const vector a) {
	int i;
	
	for (i = 0; i < M_width; i++) {
		if (a.bit[i] != 0) return i;
	}
	abort();
	return -1;
}

int
Number1Bits(const vector a, int n) {
	int res = 0;
	int i;
	
	for (i = 0; i < n; i++) {
		if (a.bit[i] != 0) {
			res++;
		}
	}
	return res;
}

void
pr_vector(const vector a) {
	int i;

	for (i = 0; i < M_width; i++) {
		printf("%c", (a.bit[i] != 0 ? '1' : '0'));
	}
}
