#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>

#include	"grammar.h"
#include	"token.h"
#include	"error.h"

/*	It is unfortunate that end of file and end of section are not
	distiguished. Better error reporting could be achieved if they were,
	but it would complicate the code.
*/

							/* TOKENS */
FILE *ifile;
const char *iname;
int line_number;

static enum line_status
get_raw_line(char *line, int line_size) {
	/* stores one raw line in 'line' or fails */
	int i;

	/* end of file? */
	if (!fgets(line, line_size, ifile)) return end_of_file;
	line_number++;

	/* end of section? */
	if (line[0] == '.' && line[1] == '\n') return end_of_section;

	/* get a complete line */
	i = 0;
	while (i < line_size && line[i] != '#' && line[i] != '\n') {
		if (line[i] == '\\' && line[i+1] == '\n') {
			if (fgets(&line[i], line_size - i, ifile)) {
				line_number++;
			}
			else {
				line[i] = '\0';
				break;
			}
		}
		else {
			i++;
		}
	}
	if (i == MAX_LINE_LENGTH)
		error_and_exit("line too long");
	line[i] = '\0';

	return line_OK;
}

static void
clean_up_spaces(char *line) {
	int i, j;
	int deleting_spaces;

	deleting_spaces = 1;
	for (i = j = 0; line[i]; i++) {
		if (isspace(line[i])) {
			if (deleting_spaces) {
			}
			else {
				deleting_spaces = 1;
				line[j++] = ' ';
			}
		}
		else {
			deleting_spaces = 0;
			line[j++] = line[i];
		}
	}
	/* remove trailing space */
	if (j > 0 && line[j-1] == ' ') {
		j--;
	}
	/* terminate line */
	line[j] = '\0';
}

static enum line_status
get_line(char *line, int line_size) {
	/* stores one non-empty normalized line in 'line' or fails */
	enum line_status ls = get_raw_line(line, line_size);

	if (ls != line_OK) return ls;

	clean_up_spaces(line);

	if (line[0] == '\0') return get_line(line, line_size);

	return line_OK;
}

static char *string[MAX_SYMBOLS];

const char *
stored_token(const char *s) {
	int i;

	for (i = 0; string[i]; i++) {
		if (strcmp(string[i], s) == 0) return string[i];
	}

	/* this is a new string */
	if (i == MAX_SYMBOLS)
		error_and_exit("too many different symbols");
	string[i] = (char *)malloc(strlen(s)+1);
	if (string[i] == 0)
		error_and_exit("out of memory");
	strcpy(string[i], s);
	return string[i];
}

const char *token[MAX_SYMBOLS];		/* on a per-line basis */
int token_ptr;

static void
add_token(const char *tk) {
	if (token_ptr == MAX_SYMBOLS)
		error_and_exit("too many tokens in a line");
	token[token_ptr++] = tk;
}

enum line_status
get_token_line(void) {
	/* reads one normalized line and tokenizes it into token[] */
	char line[MAX_LINE_LENGTH];
	enum line_status ls = get_line(line, sizeof line);
	int i, j;

	if (ls != line_OK) return ls;

	token_ptr = 0;
	/* retrieve all tokens terminated by spaces */
	for (i = j = 0; line[i]; i++) {
		/* i moves over the token, j remains pointing to its begin */
		if (line[i] == ' ') {
			line[i] = '\0';		/* mark end-of-token */
			add_token(stored_token(&line[j]));
			j = i+1;		/* start of next token */
		}
	}
	/* retrieve the last token */
	add_token(stored_token(&line[j]));

	return line_OK;
}
