#include "minishell.h"

static char	*get_pwd_expansion(char *arg, char **envp, char *env_var)
{
	char	*pwd_value;
	char	*expanded;

	pwd_value = get_env_value(env_var, envp);
	if (!pwd_value)
		return (ft_strdup(arg));
	if (*(arg + 2) == '\0')
		expanded = ft_strdup(pwd_value);
	else
		expanded = ft_strjoin(pwd_value, arg + 2);
	free(pwd_value);
	return (expanded);
}

static char	*get_home_expansion(char *arg, char **envp)
{
	char	*home;
	char	*expanded;
	char	*user;
	char	*default_home;

	home = get_env_value("HOME", envp);
	if (!home)
	{
		user = get_env_value("USER", envp);
		if (user)
		{
			default_home = ft_strjoin("/home/", user);
			free(user);
			if (default_home)
				home = default_home;
		}
	}
	if (!home)
		return (ft_strdup(arg));
	if (*(arg + 1) == '\0')
		expanded = ft_strdup(home);
	else
		expanded = ft_strjoin(home, arg + 1);
	free(home);
	return (expanded);
}

char	*expand_tilde(char *arg, char **envp)
{
	if (!arg || *arg != '~')
		return (ft_strdup(arg));
	if (*(arg + 1) == '+' && (*(arg + 2) == '\0' || *(arg + 2) == '/'))
		return (get_pwd_expansion(arg, envp, "PWD"));
	if (*(arg + 1) == '-' && (*(arg + 2) == '\0' || *(arg + 2) == '/'))
		return (get_pwd_expansion(arg, envp, "OLDPWD"));
	if (*(arg + 1) == '\0' || *(arg + 1) == '/')
		return (get_home_expansion(arg, envp));
	return (ft_strdup(arg));
}
