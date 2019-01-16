struct transition {
	char *old;
	char *input;
	char *new;
	int motion;
	char *action;
};

extern const struct transition transition[];
extern const char *initial_state;
