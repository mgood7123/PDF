/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: error.h,v 1.2 2007/12/21 16:49:47 dick Exp $
*/

extern void error(const char *module, const char *msg);
extern void error2(const char *module, const char *msg, void (*report)(void));
