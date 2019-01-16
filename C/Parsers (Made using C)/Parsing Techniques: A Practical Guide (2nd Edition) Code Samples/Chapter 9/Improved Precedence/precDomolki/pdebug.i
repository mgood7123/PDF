/* printing routines for debugging the parser */

static void
pr_stack(const char *cap, stack_entry *stack, int j) {
	int i;

	printf("Stack dump, %s:\n", cap);
	for (i = 0; i <= j; i++) {
		printf("%d: {'%c', ", i, stack[i].first);
		pr_vector(stack[i].second);
		printf("}\n");
	}
}

static void
PV(const char *cap, const vector a) {
	printf("%s: ", cap);
	pr_vector(a);
	printf("\n");
}
