#include "builtins.h"
#include "executor.h"
#include "expander.h"
#include "libft.h"
#include "minishell.h"
#include "signals.h"
#include "utils.h"
#include <sys/wait.h>
#include <unistd.h>

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

static char	*build_full_path(char *dir, char *cmd)
{
	char	*part;
	char	*full_path;

	part = ft_strjoin(dir, "/");
	if (!part)
		return (NULL);
	full_path = ft_strjoin(part, cmd);
	free(part);
	return (full_path);
}

static char	*check_path_dirs(char **dirs, char *cmd_name)
{
	char	*full_path;
	int		i;

	i = -1;
	while (dirs[++i])
	{
		full_path = build_full_path(dirs[i], cmd_name);
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

char	*exec_path(t_cmd *cmd, char **envp)
{
	char	*env_path;
	char	**dirs;

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
	return (check_path_dirs(dirs, cmd->args[0]));
}
