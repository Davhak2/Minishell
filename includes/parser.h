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
	PIPE,             // |
	AND,              // &&
	OR,               // ||
	LPAREN,           // (
	RPAREN,           // )
	EOF_,             // EOF
						/* future - > VARIABLE, EXIT_STATUS, S_QUOTE, D_QUOTE,
							WHITESPACE, NEWLINE_,
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

typedef struct s_node
{
	t_tokens		type;
	char			*value;
	struct s_node	*left;
	struct s_node	*right;
}					t_node;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
}					t_cmd;

t_token				*tokenize(char *line);

#endif // PARSER_H