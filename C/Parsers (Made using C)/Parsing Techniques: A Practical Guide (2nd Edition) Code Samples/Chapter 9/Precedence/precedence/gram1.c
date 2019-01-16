/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: gram1.c,v 1.1 2007/12/22 21:12:01 dick Exp $
*/

#include	"grammar.h"

/* BNF format */
/*
	input:		assignment_list
	assignment_list:assignment | assignment_list ';' assignment
	assignment:	[ LETTER | '^' par_expression ] '=' expression
	expression:	factor | expression ['+'|'-'] factor
	factor:		primary | factor ['*'|'/'] primary
	primary:	LETTER | DIGIT | par_expression | '^' primary
	par_expression:	'(' expression ')'
*/

/* program format */
const char *grammar[][MAX_RHS_LENGTH+3] = {
	/* the parser supplies the surrounding #s */
	{"input", ":", "#", "assignment_list", "#", 0},
	{"assignment_list", ":", "assignment", 0},
	{"assignment_list", ":", "assignment_list", ";", "assignment", 0},
	{"assignment", ":", "A", "=", "expression", 0},
	{"assignment", ":", "^", "par_expression", "=", "expression", 0},
	{"expression", ":", "factor", 0},
	{"expression", ":", "expression", "+", "factor", 0},
	{"expression", ":", "expression", "-", "factor", 0},
	{"factor", ":", "primary", 0},
	{"factor", ":", "factor", "*", "primary", 0},
	{"factor", ":", "factor", "/", "primary", 0},
	{"primary", ":", "A", 0},
	{"primary", ":", "0", 0},
	{"primary", ":", "par_expression", 0},
	{"primary", ":", "^", "primary", 0},
	{"par_expression", ":", "(", "expression", ")", 0},
	{0}
};

int
is_terminal(const char *symbol) {
	/* terminals are provisionally identified by having length 1 */
	return symbol && symbol[1] == '\0';
}
