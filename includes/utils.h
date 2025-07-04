#ifndef UTILS_H
# define UTILS_H

// Utilities and helper functions
# include "minishell.h"

// char_utils
bool	is_whitespace(char c);
bool	is_operator_char(char c);
bool	is_word_char(char c);
bool	is_quote(char c);

// token_utils
t_token *create_token(t_tokens type, char *value);
void add_token_to_list(t_token **head, t_token *new_token);
void free_token_list(t_token *head);

#endif // UTILS_H
