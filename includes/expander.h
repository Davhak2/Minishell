/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:31:00 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 17:26:27 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

void	expand_ast(t_node *node, char **envp, t_shell *shell);
void	expand_args(t_cmd *cmd, char **envp, t_shell *shell);
void	expand_redirects(t_cmd *cmd, char **envp, t_shell *shell);
void	handle_wildcard_expansion(t_cmd *cmd);
char	*get_env_value(const char *name, char **envp);
char	*expand_tilde(char *arg, char **envp);
char	*expand_word(const char *word, char **envp, int last_status);
char	*expand_var(const char *str, char **envp, int last_status);
size_t	calculate_expanded_size(const char *word, char **envp, int last_status);
char	*init_result_buffer(const char *word, char **envp, int last_status);
void	set_context_pointers(t_expand *ctx, char **result, size_t *j);
void	set_context_env(t_expand *ctx, char **envp, int last_status);
void	init_expand_context(t_expand *ctx, char **result, size_t *j);
int		handle_var_number(const char *word, size_t *i, t_expand *ctx);
void	copy_var_value(char *var_name, t_expand *ctx);
size_t	get_var_value_len(const char *word, size_t *i, char **envp,
			int last_status);
size_t	handle_question_mark(char **envp, int last_status, size_t *i);
int		wildcard_match(const char *pattern, const char *str);
char	*get_dir_from_pattern(const char *pattern);
char	*get_filename_pattern(const char *pattern);
char	**wildcard_expand(const char *pattern);
char	**resize_result_array(char **result, int count, int *cap);
int		should_skip_entry(struct dirent *entry, const char *filename_pattern);
char	*create_full_path(const char *dir_path, const char *filename);
void	iterate(int *i1, int *i2);
int		count_wildcards(t_cmd *cmd);
void	copy_wildcard_matches(char **wildcards, char **new_args,
			t_tokens *new_types, int *k);
void	copy_regular_arg(t_cmd *cmd, char **new_args,
			t_tokens *new_types, int *indices);
void	process_dollar_expansion(const char *word, size_t *i,
			size_t *total_size, t_size *ctx);
void	process_regular_char(size_t *i, size_t *total_size);

#endif // EXPANDER_H
