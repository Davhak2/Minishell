/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:28:43 by ganersis          #+#    #+#             */
/*   Updated: 2025/08/13 16:55:27 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_pipe_child1(int pipefd[2], t_shell *shell)
{
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	setup_pipe_child2(int pipefd[2], t_shell *shell)
{
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[1]);
	close(pipefd[0]);
}

void	handle_pipe_signals(int status1, int status2, t_shell *shell)
{
	if (WIFSIGNALED(status1) || WIFSIGNALED(status2))
	{
		if (WIFSIGNALED(status1))
		{
			if (WTERMSIG(status1) == SIGINT)
				write(1, "\n", 1);
			shell->last_status = 128 + WTERMSIG(status1);
		}
		else if (WIFSIGNALED(status2))
		{
			if (WTERMSIG(status2) == SIGINT)
				write(1, "\n", 1);
			shell->last_status = 128 + WTERMSIG(status2);
		}
	}
	else
		shell->last_status = WEXITSTATUS(status2);
}

void	wait_for_children(pid_t pid1, pid_t pid2, int *status1, int *status2)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid1, status1, 0);
	waitpid(pid2, status2, 0);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	execute_subshell(t_node *node, t_shell *shell, int skip_heredocs)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	pid = fork();
	if (pid == 0)
		execute_subshell_helper(&shell, &node, &exit_status, &skip_heredocs);
	else if (pid < 0)
	{
		perror("fork");
		shell->last_status = 1;
		return ;
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->last_status = WEXITSTATUS(status);
		else
			shell->last_status = 1;
	}
}
