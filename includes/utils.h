#ifndef UTILS_H
# define UTILS_H

# include "parser.h"
# include <stdbool.h>

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

void	free_shell(t_shell *shell);

// envp utils
char	**duplicate_envp(char **envp);
void	free_envp(char **envp);

#endif // UTILS_H
