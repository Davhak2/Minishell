#include "executor.h"
#include "signals.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

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

void	check_command_not_found(int status1, int status2, t_node *node)
{
	t_cmd	*cmd;

	if (WEXITSTATUS(status1) == 127)
	{
		cmd = (t_cmd *)node->left->value;
		if (cmd && cmd->args && cmd->args[0])
			exec_fail(cmd->args[0]);
	}
	if (WEXITSTATUS(status2) == 127)
	{
		cmd = (t_cmd *)node->right->value;
		if (cmd && cmd->args && cmd->args[0])
			exec_fail(cmd->args[0]);
	}
}

static void	cleanup_remaining_children(pid_t pid1, pid_t pid2, int pid1_done,
		int pid2_done)
{
	if (!pid1_done)
	{
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
	}
	if (!pid2_done)
	{
		kill(pid2, SIGTERM);
		waitpid(pid2, NULL, 0);
	}
}

void	wait_for_children(pid_t pid1, pid_t pid2, int *status1,
		int *status2)
{
	int		pid1_done;
	int		pid2_done;
	int		wait_status;
	pid_t	wait_pid;

	pid1_done = 0;
	pid2_done = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (!pid1_done || !pid2_done)
	{
		wait_pid = waitpid(-1, &wait_status, 0);
		if (wait_pid == pid1)
		{
			*status1 = wait_status;
			pid1_done = 1;
		}
		else if (wait_pid == pid2)
		{
			*status2 = wait_status;
			pid2_done = 1;
		}
		else if (wait_pid == -1)
		{
			perror("waitpid");
			break ;
		}
	}
	cleanup_remaining_children(pid1, pid2, pid1_done, pid2_done);
}
