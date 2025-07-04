#ifndef PARSER_H
#define PARSER_H

#include "minishell.h"

typedef enum e_tokens
{
	WORD,			  // CMD
	REDIRECT_IN,	  // <
	REDIRECT_OUT,	  // >
	REDIRECT_APPEND,  // >>
	REDIRECT_HEREDOC, // <<
	VARIABLE,		  // env vars
	EXIT_STATUS,	  //$?
	S_QUOTE,		  // '
	D_QUOTE,		  // "
	WHITESPACE,		  // space/tab
	NEWLINE_,		  // EOL
	EOF_,			  // EOF
	PIPE,			  // |
	AND,			  // &&
	OR,				  // ||
	LPAREN,			  // (
	RPAREN,			  // )
	CUR_DIR,		  // *
} t_tokens;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT,
	NODE_AND,
	NODE_OR,
	NODE_PARENTHESIS,
} t_node_type;

typedef struct s_token
{
	t_tokens type;
	char *value;
	struct s_token *next;
} t_token;


#endif // PARSER_H
