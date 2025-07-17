#include "executor.h"
#include "parser.h"

int exec_fail(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": command not found", 2);
	return (127);
}

char *my_getenv(char *var, char **env)
{
	int i;
	size_t len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char *exec_path(t_cmd *cmd, char **envp)
{
	char *env_path;
	char **dirs;
	char *part;
	char *full_path;
	int i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (NULL);

	// If it's a direct path
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], F_OK | X_OK) == 0)
			return (ft_strdup(cmd->args[0]));
		return (NULL);
	}

	// Get PATH from environment
	env_path = my_getenv("PATH", envp);
	if (!env_path)
		return (NULL);

	dirs = ft_split(env_path, ':');
	if (!dirs)
		return (NULL);

	i = 0;
	while (dirs[i])
	{
		part = ft_strjoin(dirs[i], "/");
		if (!part)
		{
			ft_free_array(dirs);
			return (NULL);
		}
		full_path = ft_strjoin(part, cmd->args[0]);
		free(part);
		if (!full_path)
		{
			ft_free_array(dirs);
			return (NULL);
		}
		if (access(full_path, F_OK | X_OK) == 0)
		{
			ft_free_array(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_array(dirs);
	return (NULL);
}


int execute_command(t_cmd *cmd, char **envp)
{
	pid_t pid;
	char *path;
	int status;

	if (!cmd || !cmd->cmd)
		return exec_fail("Invalid command");

	path = exec_path(cmd, envp);
	if (!path)
		return exec_fail(cmd->args[0]);
	pid = fork();
	if (!pid)
	{
		execve(path, cmd->args, envp);
		perror("execve");
		free(path);
		exit(1);
	}
	else if (pid < 0)
	{
		perror("fork");
		free(path);
		return (-1);
	}
	else
	{
		waitpid(pid, &status, 0);
		free(path);
		return WEXITSTATUS(status);
	}
}

void execute_ast(t_node *node, char **envp, int last_status)
{
	if (!node)
		return;

	if (node->type == WORD && node->value)
	{
		t_cmd *cmd = (t_cmd *)node->value;
		int status = execute_command(cmd, envp);
		last_status = status;
	}
	else if (node->type == PIPE)
	{
		int pipefd[2];
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return;
		}

		pid_t pid = fork();
		if (pid == 0)
		{
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			execute_ast(node->left, envp, last_status);
			exit(0);
		}
		else if (pid > 0)
		{
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			execute_ast(node->right, envp, last_status);
			waitpid(pid, NULL, 0);
		}
	}
	else if (node->type == AND)
	{
		execute_ast(node->left, envp, last_status);
		if (last_status == 0)
		{
			execute_ast(node->right, envp, last_status);
		}
	}
	else if (node->type == OR)
	{
		execute_ast(node->left, envp, last_status);
		if (last_status != 0)
		{
			execute_ast(node->right, envp, last_status);
		}
	}
}
