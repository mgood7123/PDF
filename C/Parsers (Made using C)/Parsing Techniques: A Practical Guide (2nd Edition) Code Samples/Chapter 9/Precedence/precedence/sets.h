/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: sets.h,v 1.2 2002/09/23 10:57:44 dick Exp $
*/

/*
	Simple management of fixed-size sets defined as
		char *set[MAX_SET_SIZE];
*/

#define	MAX_SET_SIZE	100

void add_item_to_set(const char *item, const char *set[]);
void remove_item_from_set(const char *item, const char *set[]);
int item_in_set(const char *item, const char *set[]);
