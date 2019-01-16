enum print_type {FSA, CFG, CFG_ORIG};
extern void print_symbol(const struct symbol *s);
extern void print_grammar(
	enum print_type type, const char *msg, const struct symbol *gr);
