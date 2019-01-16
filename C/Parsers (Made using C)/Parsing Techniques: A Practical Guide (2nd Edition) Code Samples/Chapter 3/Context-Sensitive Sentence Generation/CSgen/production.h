/*	This file is part of the CS sentence generator CSgen
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: production.h,v 1.3 2016-09-19 08:26:15 Gebruiker Exp $
*/

extern int r_flag;			/* for reversing the grammar */
extern int s_flag;			/* for printing sentential forms */

extern void init_production(void);
extern void produce(void);
