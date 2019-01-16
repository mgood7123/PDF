/*	This file is part of the CS sentence generator CSgen
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: production.c,v 1.5 2016-09-19 08:26:15 Gebruiker Exp $
*/

#include	<stdio.h>
#include	<stdlib.h>

#include	"gram.h"
#include	"grammar.h"
#include	"production.h"
#include	"error.h"

int r_flag;				/* for reversing the grammar */
int s_flag;				/* for printing sentential forms */

struct SF_record {
	const char **SF;		/* sentential form */
	long int SF_count;
	struct SF_record *next;
};

static struct SF_record *head;		/* of the queue */
static struct SF_record *tail;		/* of the queue */

static long int SF_count = 0;

static struct SF_record *
new_SF(int size) {
	struct SF_record *res =
		(struct SF_record *)calloc(1, sizeof(struct SF_record));
	if (!res) error("run time", "out of memory");
	res->SF = (const char **)calloc(size+1, sizeof(const char **));
	if (!res->SF) error("run time", "out of memory");
	res->SF_count = SF_count++;
	res->next = 0;
	return res;
}

static void
string_copy(const char **d, const char * const *s, int i) {
	while (i--) *d++ = *s++;
}

static int
string_length(const char * const *a) {
	int res = 0;
	while (*a++) res++;
	return res;
}

void
init_production(void) {
	int start_string_length = string_length(start_string);
	head = new_SF(start_string_length);
	string_copy(head->SF, start_string, start_string_length);
	tail = head;
}

static int
string_matches_head_at(const char **str, int i) {
	const char **SFi = &head->SF[i];

	while (*str && *SFi) {
		if (*str++ != *SFi++) return 0;
	}
	return *str == 0;
}

static void
substitute_in_head_at(const char **old, const char **new, int i) {
	const char **head_SF = head->SF;
	int length_head_SF = string_length(head_SF);

	int length_old = string_length(old);
	int length_new = string_length(new);

	int length_SF = length_head_SF - length_old + length_new;
	struct SF_record *SF_rec = new_SF(length_SF);
	const char **SFi = SF_rec->SF;	/* starts at SF_rec->SF, but moves */

	/* copy first part */
	string_copy(SFi, head_SF, i);
	SFi += i, length_SF -= i, head_SF += i;

	/* copy replacement */
	string_copy(SFi, new, length_new);
	SFi += length_new, length_SF -= length_new, head_SF += length_old;

	/* copy rest */
	string_copy(SFi, head_SF, length_SF);

	/* link in */
	tail->next = SF_rec;
	tail = SF_rec;
}

/*
  In the present algorithm, if a lhs is found N times in a sentential form,
  N new sentential forms result (with the lhs replaced by the rhs).
  As a result, terminal productions are often produced many times over,
  causing spurious ambiguity.
  It would be nice to use a canonical order, as in CF production. This,
  however, does not seem possible. For example, if we have the grammar

	S -> A A A A
	A A -> A B
	A A B A -> c c c c

   the second rule needs to be applied in a specific position to obtain a
   terminal production.
*/

static void
produce_from_head(void) {
	const char **SF = head->SF;
	int is_sentence = 1;

	int i;
	for (i = 0; SF[i]; i++) {
		if (!is_terminal(SF[i])) is_sentence = 0;
		int r;
		for (r = 0; rule[r].lhs[0]; r++) {
			const char **old =
				(!r_flag ? rule[r].lhs : rule[r].rhs);
			const char **new =
				(!r_flag ? rule[r].rhs : rule[r].lhs);
			if (!string_matches_head_at(old, i)) continue;

			/* match found */
			substitute_in_head_at(old, new, i);
			if (s_flag) {
				printf("SF %ld, from SF %ld by rule %d: ",
				       tail->SF_count, head->SF_count, r);
				print_string_of_symbols(tail->SF);
				printf("\n");
			}
		}
	}

	if (!r_flag) {
		if (is_sentence) {
			printf("SF %ld, production of start string: ",
			       head->SF_count);
			print_string_of_symbols(SF);
			printf("\n");
			fflush(stdout);
		}
	}
	if (r_flag) {
		if (string_matches_head_at(rule[0].lhs, 0)) {
			printf("SF %ld, reduction to start string: ",
			       head->SF_count);
			print_string_of_symbols(SF);
			printf("\n");
			fflush(stdout);
			head->next = 0;		/* work done */
		}
	}


	struct SF_record *head_old = head;
	head = head->next;
	free(head_old->SF);
	free(head_old);
}

void
produce(void) {
	while (head) produce_from_head();
}
