/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:30:57 by letto             #+#    #+#             */
/*   Updated: 2025/08/13 16:58:09 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

void	execute_ast(t_node *node, t_shell *shell);
void	execute_ast_internal(t_node *node, t_shell *shell, int skip_heredocs);

pid_t	create_pipe_child1(t_node *node, t_shell *shell, int pipefd[2]);
pid_t	create_pipe_child2(t_node *node, t_shell *shell, int pipefd[2],
			pid_t pid1);
void	execute_logical_ops(t_node *node, t_shell *shell, int skip_heredocs);

int		execute_command(t_cmd *cmd, t_shell *shell);

int		handle_command_fork(t_cmd *cmd, t_shell *shell, char *path);
void	handle_signal_status(int status, t_shell *shell);

int		is_builtin(char *cmd);
int		execute_builtin(t_cmd *cmd, t_shell *shell);
char	*exec_path(t_cmd *cmd, char **envp);
void	exec_fail(char *str);

int		handle_redirects(t_redirect *redirects, t_redirect_state *state,
			t_shell *shell);

int		process_all_heredocs(t_node *node, t_shell *shell,
			int *heredoc_counter);

int		handle_heredoc_fork(char *delimiter, char *filename, t_shell *shell,
			int pipefd[2]);

char	*generate_heredoc_filename(int index);
void	do_child_process(char *delimiter, char *filename, t_shell *shell,
			int pipefd[2]);

void	cleanup_heredoc_files(t_node *node);

void	setup_pipe_child1(int pipefd[2], t_shell *shell);
void	setup_pipe_child2(int pipefd[2], t_shell *shell);
void	wait_for_children(pid_t pid1, pid_t pid2, int *status1, int *status2);
void	handle_pipe_signals(int status1, int status2, t_shell *shell);

void	restore_fds(int stdin_fd, int stdout_fd, t_shell *shell);

void	execute_pipe(t_node *node, t_shell *shell);
void	execute_subshell(t_node *node, t_shell *shell, int skip_heredocs);
void	execute_subshell_helper(t_shell **shell, t_node **node,
			int *exit_status, int *skip_heredocs);

#endif // EXECUTOR_H
