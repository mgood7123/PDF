#define MAXSIZE 100                  /* some large value */

/* text handling */
char text[MAXSIZE];                  /* input text */
int length;                          /* number of symbols in text */
int tp;                              /* index in text */

static int getline(void) {
	int ch;
	printf(">"); length = 0;
	while ((ch = getchar()), ch >= 0 && ch != '\n') {
		text[length++] = ch;
	}
	text[length] = '#'; return ch == '\n';
}

/* administration of rules used */
const char *parse_list[MAXSIZE];     /* store of rules used */
int plp;                             /* index in parse_list */

static void pushrule (const char *s) { parse_list[plp++] = s; }
static void poprule(void) { --plp; }

static void parsing_found(void) {
	int i;
	printf("Derivation:\n");
	for (i = 0; i < plp; i++) printf("    %s\n", parse_list[i]);
}
