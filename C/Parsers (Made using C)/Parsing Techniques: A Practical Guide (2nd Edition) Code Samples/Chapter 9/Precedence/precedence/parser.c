/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: parser.c,v 1.4 2007/12/21 16:49:47 dick Exp $
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"precedence.h"
#include	"error.h"

							/* FAKE TEST INPUT */

static const char *input[] = {
	/* these are just tokens, thus avoiding a lexical analyser */
	"A", "=", "A", "/", "A",
	";",
	"A", "=", "(", "A", "/", "A", ")",
	";",
	"A", "=", "A",
	"#", 0
};

static int i_next;

static void
read_text(void) {
	i_next = 0;
}

static const char *
next_token(void) {
	return input[i_next++];
}

static int
char_pos(void) {
	return i_next;
}

static const char *
class_of(const char *token) {
	return token;
}

							/* STACK MANIPULATION */

#define	MAX_STACK_SIZE	200

/*	The stack format is operand (operator operand)*.  Operators that
	do not have operands on both sides are given dummy (null) operands.
*/
static const char *stack[MAX_STACK_SIZE];
static int stack_pointer;

static void
init_stack(void) {
	stack_pointer = 0;
}

static void
push(const char *item) {
	if (stack_pointer == MAX_STACK_SIZE)
		error("parser", "internal stack overflow");
	stack[stack_pointer++] = item;
}

static void
pop_entries(int n) {
	stack_pointer -= n;
	if (stack_pointer < 0)
		error("parser", "stack underflow");
}

static const char *
top_operator(void) {
	if (stack_pointer < 2) error("parser", "stack underflow");
	return stack[stack_pointer-2];
}

static void
print_stack_item(int i, int operators_only) {
	if (i % 2) {
		/* it is an operator */
		if (stack[i]) {
			printf(" %s", stack[i]);
		}
		else {
			printf(" **NO_OPERATOR**");
		}
	}
	else {	/* it is an operand */
		if (!operators_only) {
			if (stack[i]) {
				printf(" %s", stack[i]);
			}
			else {
				printf(" 0");
			}
		}
	}
}

static void
print_stack(int operators_only) {
	int i;

	printf("stack size: %d; stack:", stack_pointer);
	for (i = 0; i < stack_pointer; i++) {
		print_stack_item(i, operators_only);
	}
	printf("\n\n");
}

							/* PARSER ACTIONS */

static const char *dot;

static void
shift(void) {
	push(dot);
	push(0);
	dot = next_token();
}

static void
print_status(void) {
	printf(	"at char number %d, top operator on stack: '%s', looking at '%s'\n",
		char_pos(), top_operator(), dot ? dot : "0");
	print_stack(1);
}

static void
check(int n_rhs, const char *name, int pos) {
	/*	A rhs of length n_rhs is on top of the stack and
		its pos-th item must be equal to name.
	*/
	const char *item = stack[stack_pointer-n_rhs+pos];

	if (item == 0 || strcmp(item, name) != 0) {
		char msg[256];

		sprintf(msg, "missing %s", name);
		error2("syntax", msg, print_status);
	}
}

static void
reduce_rule(int n_rhs, const char *lhs) {
	int i;

	printf("reduce using %s:", lhs);
	for (i = 0; i < n_rhs; i++) {
		print_stack_item(stack_pointer + i - n_rhs, 0);
	}
	printf("\n");

	pop_entries(n_rhs);
	push(lhs);
}

static void
reduce(void) {
	/*	Naively we should search the grammar here; the presented
		code is already instantiated for the specific grammar,
		however, and optimized accordingly. Hence the kludges.

		Yet, searching the grammar is not particularly simple
		either: unit productions must be eliminated or at least
		unified.
	*/

	/*	Kludge: we use the fact that the first character of the
		right-most operator identifies the rule.
	*/
	switch (class_of(top_operator())[0]) {
	case '#':
		/* RHS: 0 '#' assignment '#' 0 (length = 5) */
		check(5, "assignment", 2);
		reduce_rule(5, "input");
		break;

	case '=':
		/* RHS: 0 'A' 0 '=' expression (length = 5) */
		check(5, "A", 1);
		check(5, "expression", 4);
		reduce_rule(5, "assignment");
		break;

	case ';':
		/* RHS: asignment ';' assignment (length = 3) */
		check(3, "assignment", 0);
		check(3, "assignment", 2);
		reduce_rule(3, "assignment");
		break;

	case '+':
	case '-':
	case '*':
	case '/':
		/* RHS: expression '[-|+|/|*]' expression (length = 3) */
		check(3, "expression", 0);
		check(3, "expression", 2);
		reduce_rule(3, "expression");
		break;

	case ')':
		/* RHS: 0 '(' expression ')' 0 (length = 5) */
		check(5, "(", 1);
		check(5, "expression", 2);
		reduce_rule(5, "expression");
		break;

	case 'A':
	case '0':
		/* RHS: 0 '[A|0]' 0 (length = 3) */
		reduce_rule(3, "expression");
		break;

	default:
		error("parser", "cannot find reduce rule");
		break;
	}
}

static void
parse_text(void) {
	/* initialize the parser stack */
	init_stack();
	push(0);
	dot = "#";
	shift();

	/* run the precedence parser */
	while (dot) {
		switch (precedence(class_of(top_operator()), class_of(dot))) {
		case '<':
		case '=':
			shift();
			break;
		case '>':
			reduce();
			break;
		case ' ':
			error2("syntax", "no precedence relation", print_status);
			break;
		default:
			error("parser", "illegal precedence value");
			break;
		}
	}

	/* clear the stack */
	reduce();
}

int
main(void) {
	read_text();
	parse_text();

	return 0;
}
