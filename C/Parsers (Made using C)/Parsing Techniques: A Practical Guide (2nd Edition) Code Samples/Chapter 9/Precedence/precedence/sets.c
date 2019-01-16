/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: sets.c,v 1.4 2007/12/21 16:49:48 dick Exp $
*/

#include	<string.h>

#include	"sets.h"
#include	"error.h"

/*	A set of size n-elem is implemented as an array char *S[], with
	elements S[0..n_elems-1] filled, and S[n_elems] equal to null.
*/
	

#define	streq(s1,s2)	(strcmp(s1, s2) == 0)

void
add_item_to_set(const char *item, const char *set[]) {
	int i;

	for (i = 0; set[i]; i++) {
		if (streq(item, set[i])) return;
	}
	/* found set[i] == 0; item not found */

	/* we cannot fill set[MAX_SET_SIZE-1] */
	if (i == MAX_SET_SIZE-1) error("program", "set size insufficient");
	set[i] = item;
	set[i+1] = 0;
}

void
remove_item_from_set(const char *item, const char *set[]) {
	int i;

	for (i = 0; set[i]; i++) {
		if (streq(item, set[i])) {
			int j;

			/* pull up the tail */
			for (j = i; set[j]; j++) {
				set[j] = set[j+1];
			}
			return;
		}
	}
}

int
item_in_set(const char *item, const char *set[]) {
	int i;

	for (i = 0; set[i]; i++) {
		if (streq(item, set[i])) return 1;
	}
	return 0;
}
