#include "minishell.h"

int	handle_command_fork(t_cmd *cmd, t_shell *shell, char *path)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(path, cmd->args, *(shell->envp));
		perror("execve");
		free(path);
		free_shell(shell);
		exit(1);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		g_received_signal = SIGINT;
	}
	handle_signal_status(status, shell);
	return (status);
}

void	handle_signal_status(int status, t_shell *shell)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		shell->last_status = 128 + WTERMSIG(status);
	}
	else
		shell->last_status = WEXITSTATUS(status);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
