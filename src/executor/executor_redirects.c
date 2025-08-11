#include "minishell.h"

static int	handle_output_redirect(t_redirect *current)
{
	int	fd;

	fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror("open"), -1);
	if (dup2(fd, 1) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_append_redirect(t_redirect *current)
{
	int	fd;

	fd = open(current->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (perror("open"), -1);
	if (dup2(fd, 1) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_input_redirect(t_redirect *current)
{
	int	fd;

	fd = open(current->filename, O_RDONLY);
	if (fd == -1)
		return (perror("open"), -1);
	if (dup2(fd, 0) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_heredoc_redirect(t_redirect *current)
{
	int	temp_fd;

	if (current->heredoc_filename)
	{
		temp_fd = open(current->heredoc_filename, O_RDONLY);
		if (temp_fd == -1)
		{
			perror("open temp heredoc file");
			return (1);
		}
		if (dup2(temp_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(temp_fd);
			return (1);
		}
		close(temp_fd);
	}
	return (0);
}

int	handle_redirects(t_redirect *redirects, t_redirect_state *state,
		t_shell *shell)
{
	t_redirect	*current;
	int			result;

	(void)shell;
	state->has_pipe = 0;
	current = redirects;
	while (current)
	{
		if (current->type == REDIRECT_OUT)
			result = handle_output_redirect(current);
		else if (current->type == REDIRECT_APPEND)
			result = handle_append_redirect(current);
		else if (current->type == REDIRECT_IN)
			result = handle_input_redirect(current);
		else if (current->type == REDIRECT_HEREDOC)
			result = handle_heredoc_redirect(current);
		else
			result = 0;
		if (result != 0)
			return (result);
		current = current->next;
	}
	return (0);
}
