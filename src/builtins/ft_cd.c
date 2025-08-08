#include "builtins.h"
#include "expander.h"
#include "libft.h"
#include "utils.h"
#include <unistd.h>

char	*path_join_fallback(char *base, char *rel)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(base, "/");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, rel);
	free(tmp);
	if (!result)
		return (NULL);
	return (result);
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*pwd;
	char	*oldpwd;
	char	*cwd;

	if (args[1] && args[2])
		return (ft_putstr_fd("minshell: cd: too many arguments\n", 2), 1);
	if (!args[1] || !ft_strcmp(args[1], "~"))
	{
		pwd = get_env_value("HOME", *shell->envp);
		if (!pwd)
			return (ft_putstr_fd("minshell: cd: HOME not set\n", 2), 1);
		if (chdir(pwd) == -1)
			return (free(pwd), perror("cd"), 1);
		free(pwd);
	}
	else if (!ft_strcmp(args[1], "-"))
	{
		oldpwd = get_env_value("OLDPWD", *shell->envp);
		if (!oldpwd)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
		if (chdir(oldpwd) == -1)
			return (free(oldpwd), perror("cd"), 1);
		ft_putstr_fd(oldpwd, 1);
		ft_putstr_fd("\n", 1);
		free(oldpwd);
	}
	else
	{
		if (chdir(args[1]) == -1)
			return (ft_putstr_fd("minishell: ", 2), perror("cd"), 1);
	}

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		pwd = get_env_value("PWD", *shell->envp);
		ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories\n", 2);
		if (args[1])
			cwd = path_join_fallback(pwd, args[1]);
		free(pwd);
		if (!cwd)
			return (ft_putstr_fd("cd: fallback path construction failed\n", 2), 1);
	}
	pwd = get_env_value("PWD", *shell->envp);
	if (pwd && !update_env_value("OLDPWD", pwd, shell))
		return (free(pwd), free(cwd), ft_putstr_fd("cd: failed to update OLDPWD\n", 2), 1);
	if (pwd)
		free(pwd);
	if (!update_env_value("PWD", cwd, shell))
		return (free(cwd), ft_putstr_fd("cd: failed to update PWD\n", 2), 1);
	free(cwd);
	return (0);
}
