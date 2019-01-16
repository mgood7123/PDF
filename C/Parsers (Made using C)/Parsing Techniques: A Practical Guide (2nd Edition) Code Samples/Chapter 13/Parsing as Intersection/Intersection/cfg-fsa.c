#undef	DEBUG

#include	<stdio.h>
#include	<stdlib.h>
#ifdef	DEBUG
#include	<signal.h>
#endif

#include	"cfg-fsa.h"
#include	"grammar.h"
#include	"read.h"
#include	"expand.h"
#include	"restore.h"
#include	"free.h"
#include	"print.h"
#include	"error.h"

int v_flag;
int r_flag;

#ifdef	DEBUG
static void
catch_alarm(int sig) {
	printf("BANG! signal %d\n", sig);
	exit (1);
}
#endif

static void
free_fsa(struct symbol **fsa_hook) {
	free_tree(*fsa_hook);
	*fsa_hook = 0;
}

static void
free_grammar(struct symbol **gr_hook) {
	free_tree(*gr_hook);
	*gr_hook = 0;
}

int
main(int argc, char *argv[]) {
	/* process the arguments */

	progname = argv[0]; argc--, argv++;
	while (argc > 0 && argv[0][0] == '-') {
		switch (argv[0][1]) {
		case 'r':
			r_flag = 1;
			break;
		case 'v':
			v_flag = 1;
			break;
		default:
			goto bad_call;
			break;
		}
		argc--, argv++;
	}
	if (argc > 1) goto bad_call;

	ifile = (argc > 0 ? fopen(argv[0], "r") : stdin);
	iname = (argc > 0 ? argv[0] : "stdin");
	if (ifile == 0) {
		char msg[500];

		sprintf(msg, "cannot open input file `%s'", iname);
		error_and_exit(msg);
	}

#ifdef	DEBUG
	signal(SIGSEGV, catch_alarm);
#endif

	/* read the grammar */
	if (!read_grammar()) error_and_exit("no grammar");
	print_grammar(CFG_ORIG, "Input grammar", grammar);

	/* read and process the FSAs */
	while (read_fsa()) {
		if (fsa) {
			print_grammar(FSA, "Input FSA", fsa);
			intersect();
			print_grammar(CFG, "Intersection grammar", its);
			if (r_flag) {
				restore();
				print_grammar(
					CFG, "Restored intersection grammar",
					its
				);
			}
			
			free_fsa(&fsa);
			free_grammar(&its);
		}
		else {
			error_and_exit("grammar cleaning not yet implemented");
		}
	}

	/* close down */
	if (ifile != stdin) fclose(ifile);
	free_grammar(&grammar);

	return 0;

bad_call:
	error_and_exit("Call is: cfg-fsa [-r(estore) -v(erbose)] [grammar]");
	return 1;
}
