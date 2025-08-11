/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc_fork.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:28:35 by ganersis          #+#    #+#             */
/*   Updated: 2025/08/11 19:28:36 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_signal_exit(int status)
{
	if (WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_received_signal = SIGINT;
		return (130);
	}
	if (WTERMSIG(status) == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		g_received_signal = SIGQUIT;
		return (131);
	}
	g_received_signal = WTERMSIG(status);
	return (WTERMSIG(status) + 128);
}

static int	handle_parent_wait(pid_t process_id)
{
	int	status;

	waitpid(process_id, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status))
		return (handle_signal_exit(status));
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		g_received_signal = SIGINT;
		return (130);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

int	handle_heredoc_fork(char *delimiter, char *filename, t_shell *shell,
		int pipefd[2])
{
	pid_t	process_id;

	setup_signals_parent_exec();
	process_id = fork();
	if (process_id == -1)
		return (EXIT_FAILURE);
	if (process_id == 0)
		do_child_process(delimiter, filename, shell, pipefd);
	else
		return (handle_parent_wait(process_id));
	return (EXIT_FAILURE);
}
