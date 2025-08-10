/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:31:15 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 15:31:16 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
char	*process_quotes(char **ptr, t_tokens *quote_type);
int		handle_quotes(t_token **list, char **ptr);
int		read_operator(t_type *type, char **ptr);
int		handle_word(t_token **list, char **ptr);
int		dispatch_token(t_token **list, char **ptr);
void	syntax_exit(char ptr, t_token *list);

void	free_shell(t_shell *shell);

// envp utils
char	**duplicate_envp(char **envp);
void	free_envp(char **envp);
int		update_env_value(const char *name, const char *value, t_shell *shell);

int		is_valid_identifier(char *str);
int		count_env_vars(char **envp);
int		find_env_var(char **envp, char *var_name);
int		update_env_value(const char *name, const char *value, t_shell *shell);

#endif // UTILS_H
