#include "executor.h"
#include "parser.h"

int	exec_fail(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": command not found", 2);
	return (127);
}
char	*exec_path(t_node *node, char **envp)
{
	char *p_env = NULL;
	char **paths = NULL;
	char *f_path = NULL;
	int	i;

	i = -1;
	if (!node || !node->value)
		return  (NULL);
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			p_env = envp[i] + 5;
			break;
		}
	}
	if (!p_env)
		return (NULL);
	paths = ft_split(p_env, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		f_path = ft_strjoin(paths[i], "/");
		char *temp = ft_strjoin(f_path, node->value);
		free(f_path);
		f_path = temp;

		if (!access(f_path, X_OK))
			return (ft_free_array(paths), f_path);
		free(f_path);
		f_path = NULL;
	}
	ft_free_array(paths);
	return (NULL);
}

int	execute(t_node *node, char **envp)
{
	pid_t	pid;
	char	*path;


	if (!node || node->type != WORD || !node->value)
		return exec_fail("minishell");
	path = exec_path(node, envp);
	if (!path)
		return exec_fail(node->value);
	pid = fork();
	if (!pid)
	{
		
	}
	else if (pid > 0)
	{
		wait(pid);
	}
	else
	{
		perror("fork");
		return (-1);
	}
}
