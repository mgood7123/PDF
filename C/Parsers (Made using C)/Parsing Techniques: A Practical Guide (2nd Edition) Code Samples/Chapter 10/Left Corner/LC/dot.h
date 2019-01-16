extern void start(
	const char *s,
	const char *s1,
	const char *s2,
	const char *s3,
	const char *s4
);
extern void stop(const char *s);
extern void LC_done(char *nt);
extern void term_done(char *nt);
extern void nt_done(void);
extern int yylex(void);
extern void LLmessage(int tk);
