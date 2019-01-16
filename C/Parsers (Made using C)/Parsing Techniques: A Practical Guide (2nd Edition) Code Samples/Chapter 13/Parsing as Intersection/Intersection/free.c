#include	<stdlib.h>

#include	"grammar.h"
#include	"free.h"

void
free_symbol(struct symbol *s) {
	/* clobber its contents */
	int i;
	
	for (i = 0; i < sizeof (struct symbol); i++) {
		((char *)s)[i] = 0xaa;
	}
	
	free(s);
}	

void
free_tree(struct symbol *s) {
	if (s == 0) return;
	
	while (s) {
		struct symbol *down = s->down;
		
		free_tree(s->next);
		free_symbol(s);
		
		s = down;
	}
}
