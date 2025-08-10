#include "executor.h"
#include "libft.h"
#include "minishell.h"

void	restore_fds(int stdin_fd, int stdout_fd, t_shell *shell)
{
	if (stdin_fd >= 0)
	{
		dup2(stdin_fd, 0);
		close(stdin_fd);
		shell->stdin_backup = -1;
	}
	if (stdout_fd >= 0)
	{
		dup2(stdout_fd, 1);
		close(stdout_fd);
		shell->stdout_backup = -1;
	}
}

void	exec_fail(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": command not found\n", 2);
}
