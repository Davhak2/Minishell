#include "builtins.h"

int	ft_env(char **args, char **envp)
{
	int	i;

	if (args[1])
	{
		printf("env: '%s': No such file or directory\n", args[1]);
		return (127);
	}
	i = 0;
	while (envp[i])
		printf("%s\n", envp[i++]);
	return (0);
}
