#include	"vector.h"
#include	"error.h"
#include	"stack.h"

#define	STACK_SIZE	100

stack_entry stack[STACK_SIZE];
int stack_size;

void
shift_to_the_stack(char y, const vector Q) {
	if (stack_size == STACK_SIZE) error("parser stack overflow");
	stack[stack_size].first = y;
	stack[stack_size].second = Q;
	stack_size++;
}

void
remove_from_the_stack(int r) {
	stack_size -= r;
	if (stack_size < 0) error("parser stack underflow");
}
