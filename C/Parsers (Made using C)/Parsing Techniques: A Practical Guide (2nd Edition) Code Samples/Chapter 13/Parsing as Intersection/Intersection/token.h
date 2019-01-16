/*
  Each input line is tokenized into the array token[0..token_ptr-1] by a call
  of get_token_line().  The array contains string pointers as produced by the
  symbol table function stored_token().  Identical strings produce identical
  pointers, so tokens can be compared by direct pointer comparison.
*/

enum line_status{line_OK, end_of_section, end_of_file};
extern enum line_status get_token_line(void);
extern const char *stored_token(const char *s);

extern const char *token[];
extern int token_ptr;
