#include "minishell.h"

pid_t	create_pipe_child1(t_node *node, t_shell *shell, int pipefd[2])
{
	pid_t	pid;
	int		exit_status;

	pid = fork();
	if (pid == 0)
	{
		int stderr_backup = dup(STDERR_FILENO);
		int null_fd = open("/dev/null", O_WRONLY);
		setup_pipe_child1(pipefd, shell);
		if (null_fd >= 0 && stderr_backup >= 0)
		{
			dup2(null_fd, STDERR_FILENO);
			close(null_fd);
		}
		execute_ast_internal(node->left, shell, 1);
		if (stderr_backup >= 0)
		{
			dup2(stderr_backup, STDERR_FILENO);
			close(stderr_backup);
		}
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
		int stderr_backup = dup(STDERR_FILENO);
		int null_fd = open("/dev/null", O_WRONLY);
		setup_pipe_child2(pipefd, shell);
		if (null_fd >= 0 && stderr_backup >= 0)
		{
			dup2(null_fd, STDERR_FILENO);
			close(null_fd);
		}
		execute_ast_internal(node->right, shell, 1);
		if (stderr_backup >= 0)
		{
			dup2(stderr_backup, STDERR_FILENO);
			close(stderr_backup);
		}
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
		execute_ast_internal(node->left, shell, skip_heredocs);
		if (shell->last_status == 0)
			execute_ast_internal(node->right, shell, skip_heredocs);
	}
	else if (node->type == OR)
	{
		execute_ast_internal(node->left, shell, skip_heredocs);
		if (shell->last_status != 0)
			execute_ast_internal(node->right, shell, skip_heredocs);
	}
}
