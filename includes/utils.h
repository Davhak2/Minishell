#ifndef UTILS_H
# define UTILS_H

# include "libft.h"
# include "parser.h"
# include <stdbool.h>
# include <stdlib.h>

// char_utils
bool	is_whitespace(char c);
bool	is_operator_char(char c);
bool	is_word_char(char c);
bool	is_quote(char c);

// token_utils
t_token	*create_token(t_tokens type, char *value);
void	add_token_to_list(t_token **head, t_token *new_token);
void	free_token_list(t_token *head);

// lexer_utils
t_type	set_type(char op, bool mode);
void	create_and_add(t_token **list, t_type type);

#endif // UTILS_H