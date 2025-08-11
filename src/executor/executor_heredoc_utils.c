/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:28:26 by ganersis          #+#    #+#             */
/*   Updated: 2025/08/11 19:28:26 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	here_msg(int line, char *s)
{
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(line, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("')\n", 2);
}

char	*generate_heredoc_filename(int index)
{
	char	*index_str;
	char	*filename;

	index_str = ft_itoa(index);
	if (!index_str)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", index_str);
	free(index_str);
	return (filename);
}

static void	setup_child_heredoc(t_shell *shell, int pipefd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	if (pipefd && pipefd[0] >= 0)
		close(pipefd[0]);
	if (pipefd && pipefd[1] >= 0)
		close(pipefd[1]);
	if (shell->stdin_backup >= 0)
	{
		close(shell->stdin_backup);
		shell->stdin_backup = -1;
	}
	if (shell->stdout_backup >= 0)
	{
		close(shell->stdout_backup);
		shell->stdout_backup = -1;
	}
}

static void	write_heredoc_lines(int fd, char *delimiter, t_shell *shell)
{
	char	*heredoc_line;

	while (1)
	{
		heredoc_line = readline("> ");
		if (!heredoc_line)
		{
			here_msg(shell->heredoc_line, delimiter);
			break ;
		}
		if (ft_strcmp(heredoc_line, delimiter) == 0)
		{
			free(heredoc_line);
			break ;
		}
		write(fd, heredoc_line, ft_strlen(heredoc_line));
		write(fd, "\n", 1);
		free(heredoc_line);
	}
}

void	do_child_process(char *delimiter, char *filename, t_shell *shell,
		int pipefd[2])
{
	int	fd;

	setup_child_heredoc(shell, pipefd);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		free_shell(shell);
		exit(1);
	}
	write_heredoc_lines(fd, delimiter, shell);
	close(fd);
	free_shell(shell);
	free(filename);
	exit(0);
}
