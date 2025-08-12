/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:28:41 by ganersis          #+#    #+#             */
/*   Updated: 2025/08/11 19:28:41 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	create_pipe_child1(t_node *node, t_shell *shell, int pipefd[2])
{
	pid_t	pid;
	int		exit_status;

	pid = fork();
	if (pid == 0)
	{
		setup_pipe_child1(pipefd, shell);
		execute_ast_internal(node->left, shell, 1);
		exit_status = shell->last_status;
		free_shell(shell);
		exit(exit_status);
	}
	else if (pid < 0)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (pid);
}

pid_t	create_pipe_child2(t_node *node, t_shell *shell, int pipefd[2],
		pid_t pid1)
{
	pid_t	pid;
	int		exit_status;

	pid = fork();
	if (pid == 0)
	{
		setup_pipe_child2(pipefd, shell);
		execute_ast_internal(node->right, shell, 1);
		exit_status = shell->last_status;
		free_shell(shell);
		exit(exit_status);
	}
	else if (pid < 0)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
	}
	return (pid);
}

void	execute_logical_ops(t_node *node, t_shell *shell, int skip_heredocs)
{
	if (node->type == AND)
	{
		(void)skip_heredocs;
		execute_ast_internal(node->left, shell, 1);
		if (shell->last_status == 0)
			execute_ast_internal(node->right, shell, 1);
	}
	else if (node->type == OR)
	{
		(void)skip_heredocs;
		execute_ast_internal(node->left, shell, 1);
		if (shell->last_status != 0)
			execute_ast_internal(node->right, shell, 1);
	}
}
