#include "builtins.h"
#include "executor.h"
#include "expander.h"
#include "libft.h"
#include "parser.h"
#include "utils.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int	exec_fail(char *str)
{
	printf("minishell: %s: command not found\n", str);
	return (127);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_cmd *cmd, char **envp)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(cmd->args));
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(cmd->args, envp));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd(envp));
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(cmd->args));
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(cmd->args, envp));
	return (1);
}

int	handle_redirects(t_redirect *redirects, t_redirect_state *state)
{
	t_redirect	*current;
	int			fd;
	char		buffer[1024];
	char		line_buffer[1024];
	int			line_pos;
	ssize_t		bytes_read;
	int			i;

	state->has_pipe = 0;
	current = redirects;
	while (current)
	{
		if (current->type == REDIRECT_OUT)
		{
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
		}
		else if (current->type == REDIRECT_APPEND)
		{
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
		}
		else if (current->type == REDIRECT_IN)
		{
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
		}
		else if (current->type == REDIRECT_HEREDOC)
		{
			if (pipe(state->pipefd) == -1)
			{
				perror("pipe");
				return (-1);
			}
			state->has_pipe = 1;
			while (1)
			{
				write(STDOUT_FILENO, "> ", 2);
				line_pos = 0;
				while (1)
				{
					bytes_read = read(STDIN_FILENO, buffer, 1);
					if (bytes_read <= 0)
					{
						close(state->pipefd[0]);
						close(state->pipefd[1]);
						return (-1);
					}
					if (buffer[0] == '\n')
						break ;
					if (line_pos < 1023)
					{
						line_buffer[line_pos] = buffer[0];
						line_pos++;
					}
				}
				line_buffer[line_pos] = '\0';
				if (ft_strcmp(line_buffer, current->filename) == 0)
					break ;
				i = 0;
				while (line_buffer[i])
				{
					write(state->pipefd[1], &line_buffer[i], 1);
					i++;
				}
				write(state->pipefd[1], "\n", 1);
			}
			close(state->pipefd[1]);
			if (dup2(state->pipefd[0], STDIN_FILENO) == -1)
			{
				perror("dup2");
				close(state->pipefd[0]);
				return (-1);
			}
			close(state->pipefd[0]);
			state->has_pipe = 0;
		}
		current = current->next;
	}
	return (0);
}

char	*exec_path(t_cmd *cmd, char **envp)
{
	char	*env_path;
	char	**dirs;
	char	*part;
	char	*full_path;
	int		i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (NULL);
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], F_OK | X_OK) == 0)
			return (ft_strdup(cmd->args[0]));
		return (NULL);
	}
	env_path = get_env_value("PATH", envp);
	if (!env_path)
		return (NULL);
	dirs = ft_split(env_path, ':');
	if (!dirs)
		return (NULL);
	free(env_path);
	i = -1;
	while (dirs[++i])
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
	}
	ft_free_array(dirs);
	return (NULL);
}

int	execute_command(t_cmd *cmd, char **envp)
{
	pid_t				pid;
	char				*path;
	int					status;
	int					redirect_mode;
	int					stdin_fd;
	int					stdout_fd;
	t_redirect_state	state;

	redirect_mode = 0;
	if (!cmd || !cmd->cmd)
		return (exec_fail("Invalid command"));
	if (cmd->redirects)
	{
		stdin_fd = dup(0);
		stdout_fd = dup(1);
		redirect_mode = 1;
		if (handle_redirects(cmd->redirects, &state) == -1)
		{
			dup2(stdin_fd, 0);
			dup2(stdout_fd, 1);
			close(stdin_fd);
			close(stdout_fd);
			return (1);
		}
	}
	if (is_builtin(cmd->args[0]))
	{
		status = execute_builtin(cmd, envp);
		if (redirect_mode)
		{
			dup2(stdin_fd, 0);
			dup2(stdout_fd, 1);
			close(stdin_fd);
			close(stdout_fd);
		}
		return (status);
	}
	path = exec_path(cmd, envp);
	if (!path)
	{
		if (redirect_mode)
		{
			dup2(stdin_fd, 0);
			dup2(stdout_fd, 1);
			close(stdin_fd);
			close(stdout_fd);
		}
		return (exec_fail(cmd->args[0]));
	}
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
		if (redirect_mode)
		{
			dup2(stdin_fd, 0);
			dup2(stdout_fd, 1);
			close(stdin_fd);
			close(stdout_fd);
		}
		return (-1);
	}
	else
	{
		waitpid(pid, &status, 0);
		free(path);
	}
	if (redirect_mode)
	{
		dup2(stdin_fd, 0);
		dup2(stdout_fd, 1);
		close(stdin_fd);
		close(stdout_fd);
	}
	return (WEXITSTATUS(status));
}

void	execute_ast(t_node *node, char **envp, int last_status, t_shell *shell)
{
	t_cmd	*cmd;
	int		status;
	int		pipefd[2];

	if (!node)
		return ;
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		status = execute_command(cmd, envp);
		last_status = status;
	}
	else if (node->type == PIPE)
	{
		pid_t pid1, pid2;
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return ;
		}
		pid1 = fork();
		if (pid1 == 0)
		{
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			execute_ast(node->left, envp, last_status, shell);
			free_shell(shell);
			exit(0);
		}
		else if (pid1 < 0)
		{
			perror("fork");
			close(pipefd[0]);
			close(pipefd[1]);
			return ;
		}
		pid2 = fork();
		if (pid2 == 0)
		{
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[1]);
			close(pipefd[0]);
			execute_ast(node->right, envp, last_status, shell);
			free_shell(shell);
			exit(0);
		}
		else if (pid2 < 0)
		{
			perror("fork");
			close(pipefd[0]);
			close(pipefd[1]);
			waitpid(pid1, NULL, 0);
			return ;
		}
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(pid1, NULL, 0);
		waitpid(pid2, &status, 0);
		last_status = WEXITSTATUS(status);
	}
	else if (node->type == AND)
	{
		execute_ast(node->left, envp, last_status, shell);
		if (last_status == 0)
			execute_ast(node->right, envp, last_status, shell);
	}
	else if (node->type == OR)
	{
		execute_ast(node->left, envp, last_status, shell);
		if (last_status != 0)
			execute_ast(node->right, envp, last_status, shell);
	}
}
