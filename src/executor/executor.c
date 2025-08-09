#include "builtins.h"
#include "executor.h"
#include "expander.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include "signals.h"
#include "utils.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

static void	do_child_process(char *delimiter, char *filename, t_shell *shell,
				int pipefd[2]);
static int	handle_heredoc_fork(char *delimiter, char *filename, t_shell *shell,
				int pipefd[2]);

static void	restore_fds(int stdin_fd, int stdout_fd, t_shell *shell)
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
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(cmd->args, shell));
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(cmd->args, shell));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd(*(shell->envp)));
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(cmd->args, shell));
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(cmd->args, shell));
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(cmd->args, shell));
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(cmd->args, *(shell->envp)));
	return (1);
}

int	handle_redirects(t_redirect *redirects, t_redirect_state *state,
		t_shell *shell)
// TODO: fix here-doc redirection,"cat << end > a.txt"
{
	t_redirect *current;
	int fd;
	int tty_fd;
	char buffer[1024];
	char line[1024];
	int line_pos;
	ssize_t bytes_read;
	char *heredoc_line;
	t_redirect *next_redirect;
	int is_last_heredoc;
	int temp_fd;
	int heredoc_result;

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
		// TODO: grep hi <./test_files/infile_big <./test_files/infile segfault ?????
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
			is_last_heredoc = 1;
			next_redirect = current->next;
			while (next_redirect)
			{
				if (next_redirect->type == REDIRECT_HEREDOC)
				{
					is_last_heredoc = 0;
					break ;
				}
				next_redirect = next_redirect->next;
			}
			heredoc_result = handle_heredoc_fork(current->filename,
					"/tmp/minishell_heredoc", shell, NULL);
			if (heredoc_result != 0)
			{
				unlink("/tmp/minishell_heredoc");
				return (heredoc_result);
			}
			if (is_last_heredoc)
			{
				temp_fd = open("/tmp/minishell_heredoc", O_RDONLY);
				if (temp_fd == -1)
				{
					perror("open temp heredoc file");
					unlink("/tmp/minishell_heredoc");
					return (1);
				}
				if (dup2(temp_fd, STDIN_FILENO) == -1)
				{
					perror("dup2");
					close(temp_fd);
					unlink("/tmp/minishell_heredoc");
					return (1);
				}
				close(temp_fd);
			}
			unlink("/tmp/minishell_heredoc");
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

int	execute_command(t_cmd *cmd, t_shell *shell)
{
	pid_t				pid;
	char				*path;
	int					status;
	int					redirect_mode;
	int					stdin_fd;
	int					stdout_fd;
	t_redirect_state	state;

	stdin_fd = -1;
	stdout_fd = -1;
	redirect_mode = 0;
	if (!cmd)
		return (127);
	if (cmd->redirects)
	{
		stdin_fd = dup(0);
		stdout_fd = dup(1);
		shell->stdin_backup = stdin_fd;
		shell->stdout_backup = stdout_fd;
		redirect_mode = 1;
		status = handle_redirects(cmd->redirects, &state, shell);
		if (status != 0)
		{
			restore_fds(stdin_fd, stdout_fd, shell);
			return (status > 0 ? status : 1);
		}
	}
	if (!cmd->cmd || !cmd->args || !cmd->args[0])
	{
		if (redirect_mode)
			restore_fds(stdin_fd, stdout_fd, shell);
		return (0);
	}
	if (is_builtin(cmd->args[0]))
	{
		status = execute_builtin(cmd, shell);
		if (redirect_mode)
			restore_fds(stdin_fd, stdout_fd, shell);
		return (status);
	}
	path = exec_path(cmd, *(shell->envp));
	if (!path)
	{
		exec_fail(cmd->args[0]);
		if (redirect_mode)
			restore_fds(stdin_fd, stdout_fd, shell);
		return (127);
	}
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
		free(path);
		if (redirect_mode)
			restore_fds(stdin_fd, stdout_fd, shell);
		return (-1);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				write(1, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				write(1, "Quit (core dumped)\n", 19);
			shell->last_status = 128 + WTERMSIG(status);
		}
		else
		{
			shell->last_status = WEXITSTATUS(status);
		}
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		free(path);
	}
	if (redirect_mode)
		restore_fds(stdin_fd, stdout_fd, shell);
	return (WEXITSTATUS(status));
}

static void	here_msg(int line, char *s)
{
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(line, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("')\n", 2);
}

static void	do_child_process(char *delimiter, char *filename, t_shell *shell,
		int pipefd[2])
{
	char	*heredoc_line;
	int		fd;

	rl_catch_signals = 0;
	g_received_signal = 0;
	signal(SIGINT, sigint_heredoc_handler);
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
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		free_shell(shell);
		exit(1);
	}
	while (1)
	{
		heredoc_line = readline("> ");
		if (g_received_signal == SIGINT)
		{
			// write(STDOUT_FILENO, "\n", 1);
			close(fd);
			unlink(filename);
			free_shell(shell);
			exit(130);
		}
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
	close(fd);
	free_shell(shell);
	exit(0);
}

static int	handle_heredoc_fork(char *delimiter, char *filename, t_shell *shell,
		int pipefd[2])
{
	pid_t	process_id;
	int		status;
	char	nl;

	setup_signals_parent_exec();
	process_id = fork();
	if (process_id == -1)
		return (EXIT_FAILURE);
	if (process_id == 0)
		do_child_process(delimiter, filename, shell, pipefd);
	else
	{
		waitpid(process_id, &status, 0);
		setup_signals();
		if (WIFSIGNALED(status))
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
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			g_received_signal = SIGINT;
			// nl = '\n';
			// ioctl(STDIN_FILENO, TIOCSTI, &nl);
			return (130);
		}
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (EXIT_FAILURE);
}

void	execute_ast(t_node *node, t_shell *shell)
{
	t_cmd		*cmd;
	int			status;
	int			pipefd[2];
	int			heredoc_status;
	int			right_has_heredoc;
	t_redirect	*current;
	t_redirect	*last_heredoc;
	int			exit_status;
	int			temp_fd;
	int			dev_null;
	int			has_heredoc;
	char		*path;
	int			pid1_done;
	int			pid2_done;
	int			wait_status;
	pid_t		wait_pid;

	if (!node)
		return ;
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		expand_ast(node, *(shell->envp), shell);
		status = execute_command(cmd, shell);
		shell->last_status = status;
	}
	else if (node->type == PIPE)
	{
		pid_t pid1, pid2;
		int status1, status2;
		t_cmd *left_cmd, *right_cmd;
		heredoc_status = 0;
		right_has_heredoc = 0;
		if (node->right && node->right->type == WORD && node->right->value)
		{
			right_cmd = (t_cmd *)node->right->value;
			if (right_cmd->redirects)
			{
				current = right_cmd->redirects;
				while (current)
				{
					if (current->type == REDIRECT_HEREDOC)
					{
						right_has_heredoc = 1;
						break ;
					}
					current = current->next;
				}
			}
		}
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return ;
		}
		if (node->left && node->left->type == WORD && node->left->value)
		{
			left_cmd = (t_cmd *)node->left->value;
			if (left_cmd->redirects)
			{
				current = left_cmd->redirects;
				last_heredoc = NULL;
				while (current)
				{
					if (current->type == REDIRECT_HEREDOC)
						last_heredoc = current;
					current = current->next;
				}
				if (last_heredoc)
				{
					heredoc_status = handle_heredoc_fork(last_heredoc->filename,
							"/tmp/minishell_heredoc", shell, pipefd);
					if (heredoc_status != 0)
					{
						unlink("/tmp/minishell_heredoc");
						close(pipefd[0]);
						close(pipefd[1]);
						shell->last_status = heredoc_status;
						return ;
					}
				}
			}
		}
		pid1 = fork();
		if (pid1 == 0)
		{
			shell->stdin_backup = -1;
			shell->stdout_backup = -1;
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (right_has_heredoc)
			{
				dev_null = open("/dev/null", O_WRONLY);
				if (dev_null != -1)
				{
					dup2(dev_null, STDOUT_FILENO);
					close(dev_null);
				}
			}
			else
			{
				dup2(pipefd[1], STDOUT_FILENO);
			}
			close(pipefd[0]);
			close(pipefd[1]);
			if (node->left && node->left->type == WORD && node->left->value)
			{
				left_cmd = (t_cmd *)node->left->value;
				if (left_cmd->redirects)
				{
					current = left_cmd->redirects;
					has_heredoc = 0;
					while (current)
					{
						if (current->type == REDIRECT_HEREDOC)
						{
							temp_fd = open("/tmp/minishell_heredoc", O_RDONLY);
							if (temp_fd != -1)
							{
								dup2(temp_fd, STDIN_FILENO);
								close(temp_fd);
								has_heredoc = 1;
							}
							break ;
						}
						current = current->next;
					}
					if (has_heredoc)
					{
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
						if (is_builtin(left_cmd->args[0]))
						{
							exit_status = execute_builtin(left_cmd, shell);
						}
						else
						{
							path = exec_path(left_cmd, *(shell->envp));
							if (path)
							{
								execve(path, left_cmd->args, *(shell->envp));
								free(path);
							}
							exit_status = 127;
						}
						unlink("/tmp/minishell_heredoc");
						free_shell(shell);
						exit(exit_status);
					}
				}
			}
			execute_ast(node->left, shell);
			exit_status = shell->last_status;
			unlink("/tmp/minishell_heredoc");
			free_shell(shell);
			exit(exit_status);
		}
		else if (pid1 < 0)
		{
			perror("fork");
			close(pipefd[0]);
			close(pipefd[1]);
			unlink("/tmp/minishell_heredoc");
			return ;
		}
		pid2 = fork();
		if (pid2 == 0)
		{
			shell->stdin_backup = -1;
			shell->stdout_backup = -1;
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (!right_has_heredoc)
			{
				dup2(pipefd[0], STDIN_FILENO);
			}
			close(pipefd[1]);
			close(pipefd[0]);
			execute_ast(node->right, shell);
			exit_status = shell->last_status;
			unlink("/tmp/minishell_heredoc");
			free_shell(shell);
			exit(exit_status);
		}
		else if (pid2 < 0)
		{
			perror("fork");
			close(pipefd[0]);
			close(pipefd[1]);
			kill(pid1, SIGTERM);
			waitpid(pid1, NULL, 0);
			return ;
		}
		close(pipefd[0]);
		close(pipefd[1]);
		pid1_done = 0;
		pid2_done = 0;
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		while (!pid1_done || !pid2_done)
		{
			wait_pid = waitpid(-1, &wait_status, 0);
			if (wait_pid == pid1)
			{
				status1 = wait_status;
				pid1_done = 1;
			}
			else if (wait_pid == pid2)
			{
				status2 = wait_status;
				pid2_done = 1;
			}
			else if (wait_pid == -1)
			{
				perror("waitpid");
				break ;
			}
		}
		if (!pid1_done)
		{
			kill(pid1, SIGTERM);
			waitpid(pid1, &status1, 0);
		}
		if (!pid2_done)
		{
			kill(pid2, SIGTERM);
			waitpid(pid2, &status2, 0);
		}
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
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
		unlink("/tmp/minishell_heredoc");
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
	else if (node->type == AND)
	{
		execute_ast(node->left, shell);
		if (shell->last_status == 0)
			execute_ast(node->right, shell);
	}
	else if (node->type == OR)
	{
		execute_ast(node->left, shell);
		if (shell->last_status != 0)
			execute_ast(node->right, shell);
	}
}
