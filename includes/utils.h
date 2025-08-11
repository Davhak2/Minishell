/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:31:15 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 19:45:15 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

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
void	syntax_exit(char ptr, t_token *list);

void	free_shell(t_shell *shell);

// parser_utils
void	syntax_error(char *str);
void	free_ast(t_node *node);
void	free_redirects(t_redirect *redir);
int		is_arg_token(t_tokens t);
int		is_redir_token(t_tokens t);
int		in_simple_span(t_tokens t);

// envp utils
char	**duplicate_envp(char **envp);
void	free_envp(char **envp);
int		update_env_value(const char *name, const char *value, t_shell *shell);
int		has_special_chars(char *str);

// builtin utils
int		is_valid_identifier(char *str);
int		count_env_vars(char **envp);
int		find_env_var(char **envp, char *var_name);
int		update_env_value(const char *name, const char *value, t_shell *shell);
int		env_len(char **envp);
int		matches_name(const char *entry, const char *name);
int		replace_in_place(char ***envp_ref, int idx, char *new_var);

// main_utils
bool	has_unclosed_quote(const char *str);
char	*handle_multiline_input(char *input, t_shell *shell);
int	validate_and_parse(t_shell *shell, t_token **tokens, t_node **ast);
void	cleanup_resources(t_node *ast, t_token *tokens, t_shell *shell);
t_shell	*init_minishell(char **envp, char ***my_envp);
int		handle_input_line(t_shell *shell);
int		process_input(char *input, t_shell *shell);

#endif // UTILS_H
