typedef struct {
	char first;
	vector second;
} stack_entry;

extern stack_entry stack[];
extern int stack_size;

#define	top_of_stack	(stack+stack_size-1)

extern void shift_to_the_stack(char y, const vector Q);
extern void remove_from_the_stack(int r);
