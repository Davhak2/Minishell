#include "builtins.h"
#include "expander.h"
#include "stdio.h"

#include "libft.h"

char	*expand_tilde(char *arg, char **envp)
{
	char	*home;
	char	*expanded;
	char	*user;
	char	*default_home;
	char	*pwd_value;

	if (!arg || *arg != '~')
		return (ft_strdup(arg));
	if (*(arg + 1) == '+' && (*(arg + 2) == '\0' || *(arg + 2) == '/'))
	{
		pwd_value = get_env_value("PWD", envp);
		if (!pwd_value)
			return (ft_strdup(arg));

		if (*(arg + 2) == '\0')
			expanded = ft_strdup(pwd_value);
		else
			expanded = ft_strjoin(pwd_value, arg + 2);
		free(pwd_value);
		return (expanded);
	}
	if (*(arg + 1) == '-' && (*(arg + 2) == '\0' || *(arg + 2) == '/'))
	{
		pwd_value = get_env_value("OLDPWD", envp);
		if (!pwd_value)
			return (ft_strdup(arg));

		if (*(arg + 2) == '\0')
			expanded = ft_strdup(pwd_value);
		else
			expanded = ft_strjoin(pwd_value, arg + 2);
		free(pwd_value);
		return (expanded);
	}
	if (*(arg + 1) == '\0' || *(arg + 1) == '/')
	{
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
	return (ft_strdup(arg));
}

int	ft_echo(char **args, t_shell *shell)
{
	int		i;
	int		newline;
	char	*expanded;

	i = 1;
	newline = 1;
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}

	while (args[i])
	{
		if (args[i][0] == '~' && shell && shell->envp)
		{
			expanded = expand_tilde(args[i], *(shell->envp));
			printf("%s", expanded);
			free(expanded);
		}
		else
			printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
