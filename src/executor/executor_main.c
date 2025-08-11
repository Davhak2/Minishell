/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:28:39 by ganersis          #+#    #+#             */
/*   Updated: 2025/08/11 19:28:39 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredocs_if_needed(t_node *node, t_shell *shell,
		int skip_heredocs)
{
	int	heredoc_counter;
	int	heredoc_status;

	if (skip_heredocs)
		return (0);
	heredoc_counter = 0;
	heredoc_status = process_all_heredocs(node, shell, &heredoc_counter);
	if (heredoc_status != 0)
	{
		shell->last_status = heredoc_status;
		return (1);
	}
	return (0);
}

static void	execute_pipe(t_node *node, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		shell->last_status = 1;
		return ;
	}
	pid1 = create_pipe_child1(node, shell, pipefd);
	pid2 = create_pipe_child2(node, shell, pipefd, pid1);
	close(pipefd[0]);
	close(pipefd[1]);
	if (pid1 > 0 && pid2 > 0)
	{
		wait_for_children(pid1, pid2, &status1, &status2);
		handle_pipe_signals(status1, status2, shell);
	}
	else
		shell->last_status = 1;
}

void	execute_ast_internal(t_node *node, t_shell *shell, int skip_heredocs)
{
	t_cmd	*cmd;
	int		status;

	if (!node)
		return ;
	if (handle_heredocs_if_needed(node, shell, skip_heredocs))
		return ;
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		expand_ast(node, *(shell->envp), shell);
		status = execute_command(cmd, shell);
		shell->last_status = status;
	}
	else if (node->type == PIPE)
		execute_pipe(node, shell);
	else if (node->type == AND || node->type == OR)
		execute_logical_ops(node, shell, skip_heredocs);
	if (!skip_heredocs)
		cleanup_heredoc_files(node);
}

void	execute_ast(t_node *node, t_shell *shell)
{
	execute_ast_internal(node, shell, 0);
}
