#include "executor.h"
#include "signals.h"
#include "utils.h"
#include <sys/wait.h>
#include <unistd.h>

static int	setup_redirects_and_check_cmd(t_cmd *cmd, t_shell *shell,
		int *stdin_fd, int *stdout_fd)
{
	t_redirect_state	state;
	int					status;

	*stdin_fd = -1;
	*stdout_fd = -1;
	if (cmd->redirects)
	{
		*stdin_fd = dup(0);
		*stdout_fd = dup(1);
		shell->stdin_backup = *stdin_fd;
		shell->stdout_backup = *stdout_fd;
		status = handle_redirects(cmd->redirects, &state, shell);
		if (status != 0)
		{
			if (status > 0)
				return (status);
			return (1);
		}
	}
	if (!cmd->cmd || !cmd->args || !cmd->args[0])
		return (0);
	return (-1);
}

static void	restore_fds_if_needed(int stdin_fd, int stdout_fd, t_shell *shell)
{
	if (stdin_fd >= 0 || stdout_fd >= 0)
		restore_fds(stdin_fd, stdout_fd, shell);
}

static int	execute_external_command(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	int		status;

	path = exec_path(cmd, *(shell->envp));
	if (!path)
	{
		exec_fail(cmd->args[0]);
		return (127);
	}
	status = handle_command_fork(cmd, shell, path);
	free(path);
	return (shell->last_status);
}

int	execute_command(t_cmd *cmd, t_shell *shell)
{
	int	status;
	int	stdin_fd;
	int	stdout_fd;

	if (!cmd)
		return (127);
	status = setup_redirects_and_check_cmd(cmd, shell, &stdin_fd, &stdout_fd);
	if (status >= 0)
	{
		restore_fds_if_needed(stdin_fd, stdout_fd, shell);
		return (status);
	}
	if (is_builtin(cmd->args[0]))
	{
		status = execute_builtin(cmd, shell);
		restore_fds_if_needed(stdin_fd, stdout_fd, shell);
		return (status);
	}
	status = execute_external_command(cmd, shell);
	restore_fds_if_needed(stdin_fd, stdout_fd, shell);
	return (status);
}
