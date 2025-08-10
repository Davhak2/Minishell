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
static int	process_all_heredocs(t_node *node, t_shell *shell, int *heredoc_counter);
static int	process_heredocs_in_cmd(t_cmd *cmd, t_shell *shell, int *heredoc_counter);
static char	*generate_heredoc_filename(int index);
static void	execute_ast_internal(t_node *node, t_shell *shell, int skip_heredocs);
static void	cleanup_heredoc_files(t_node *node);

static void	cleanup_heredoc_files(t_node *node)
{
	t_cmd	*cmd;
	t_redirect *current;

	if (!node)
		return ;

	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		if (cmd && cmd->redirects)
		{
			current = cmd->redirects;
			while (current)
			{
				if (current->type == REDIRECT_HEREDOC && current->heredoc_filename)
				{
					unlink(current->heredoc_filename);
				}
				current = current->next;
			}
		}
	}
	else if (node->type == PIPE || node->type == AND || node->type == OR)
	{
		cleanup_heredoc_files(node->left);
		cleanup_heredoc_files(node->right);
	}
}

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
// TODO: fix here-doc redirection,"cat << end > a.txt "
{
	t_redirect *current;
	int fd;
	int temp_fd;

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
		// TODO: grep hi <./test_files/infile_big <./test_files/infile segfault ????
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

static char	*generate_heredoc_filename(int index)
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

static int	process_heredocs_in_cmd(t_cmd *cmd, t_shell *shell, int *heredoc_counter)
{
	t_redirect	*current;
	int			result;
	char		*filename;

	if (!cmd || !cmd->redirects)
		return (0);
	current = cmd->redirects;
	while (current)
	{
		if (current->type == REDIRECT_HEREDOC)
		{
			filename = generate_heredoc_filename((*heredoc_counter)++);
			if (!filename)
				return (1);
			result = handle_heredoc_fork(current->filename, filename, shell, NULL);
			if (result != 0)
			{
				unlink(filename);
				free(filename);
				return (result);
			}
			free(current->heredoc_filename);
			current->heredoc_filename = filename;
		}
		current = current->next;
	}
	return (0);
}

static int	process_all_heredocs(t_node *node, t_shell *shell, int *heredoc_counter)
{
	int		result;
	t_cmd	*cmd;

	if (!node)
		return (0);
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		return (process_heredocs_in_cmd(cmd, shell, heredoc_counter));
	}
	else if (node->type == PIPE || node->type == AND || node->type == OR)
	{
		result = process_all_heredocs(node->left, shell, heredoc_counter);
		if (result != 0)
			return (result);
		return (process_all_heredocs(node->right, shell, heredoc_counter));
	}
	return (0);
}

void	execute_ast(t_node *node, t_shell *shell)
{
	execute_ast_internal(node, shell, 0);
}

static void	execute_ast_internal(t_node *node, t_shell *shell, int skip_heredocs)
{
	t_cmd		*cmd;
	int			status;
	int			pipefd[2];
	int			heredoc_status;
	int			exit_status;
	int			pid1_done;
	int			pid2_done;
	int			wait_status;
	pid_t		wait_pid;
	int			heredoc_counter;

	if (!node)
		return ;

	// Only process heredocs at the top level (not in child processes)
	if (!skip_heredocs)
	{
		// Initialize heredoc counter for this execution
		heredoc_counter = 0;

		// Process all heredocs first, before executing any commands
		heredoc_status = process_all_heredocs(node, shell, &heredoc_counter);
		if (heredoc_status != 0)
		{
			shell->last_status = heredoc_status;
			return ;
		}
	}
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

		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return ;
		}

		pid1 = fork();
		if (pid1 == 0)
		{
			shell->stdin_backup = -1;
			shell->stdout_backup = -1;
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			execute_ast_internal(node->left, shell, 1);
			exit_status = shell->last_status;
			free_shell(shell);
			exit(exit_status);
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
			shell->stdin_backup = -1;
			shell->stdout_backup = -1;
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[1]);
			close(pipefd[0]);
			execute_ast_internal(node->right, shell, 1);
			exit_status = shell->last_status;
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

	// Clean up heredoc files only at the top level
	if (!skip_heredocs)
	{
		cleanup_heredoc_files(node);
	}
}
