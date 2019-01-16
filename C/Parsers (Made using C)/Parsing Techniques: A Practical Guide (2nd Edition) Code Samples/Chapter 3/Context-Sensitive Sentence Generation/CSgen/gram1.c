/*	This file is part of the CS sentence generator CSgen
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: gram1.c,v 1.4 2016-09-16 20:05:36 Gebruiker Exp $
*/

#include	"gram.h"

const char *terminal[] = {"a", "b", "c", 0};

struct rule rule[] = {
	{{"S"}, {"#", "<", "T", "#"}},
	{{"T"}, {"A", "B", "C", "T"}},
	{{"T"}, {"A", "B", "C"}},

	{{"B", "A"}, {"A", "B"}},
	{{"C", "A"}, {"A", "C"}},
	{{"C", "B"}, {"B", "C"}},

	{{"#", "<", "A"}, {"a", "<"}},
	{{"a", "<", "A"}, {"a", "a", "<"}},
	{{"a", "<", "B"}, {"a", "b", "<"}},
	{{"b", "<", "B"}, {"b", "b", "<"}},
	{{"b", "<", "C"}, {"b", "c", "<"}},
	{{"c", "<", "C"}, {"c", "c", "<"}},
	{{"<", "#"}, {0 /*??*/}},
	0
};

const char *start_string[] = {"S", 0};
