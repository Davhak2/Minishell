#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>
# include <stdlib.h>

typedef enum e_tokens
{
	WORD,             // CMD, arguments, filenames
	REDIRECT_IN,      // <
	REDIRECT_OUT,     // >
	REDIRECT_APPEND,  // >>
	REDIRECT_HEREDOC, // <<
	PIPE,             // | (для pipes)
	AND,              // && (добавим позже)
	OR,               // || (добавим позже)
	LPAREN,           // ( (для приоритетов)
	RPAREN,           // ) (для приоритетов)
	EOF_,             // EOF
	/* future - > VARIABLE, EXIT_STATUS, S_QUOTE, D_QUOTE, WHITESPACE, NEWLINE_,
		CUR_DIR*/
}					t_tokens;

typedef struct s_token
{
	t_tokens		type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_type
{
	t_tokens		type;
	char			*value;
}					t_type;

t_token				*tokenize(char *line);

#endif // PARSER_H