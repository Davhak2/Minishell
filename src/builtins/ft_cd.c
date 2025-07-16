#include "builtins.h"
#include "expander.h"
#include <unistd.h>

int	ft_cd(char **args, char **envp) //TODO: NOT WORKING
{
	char *cwd;

	cwd = ft_strdup("HOME");
	chdir(get_env_value(cwd, envp));
	for (int i = 0; envp[i]; i++)
	{
		if (ft_strncmp(envp[i], "PWD=", 4) == 0)
		{
			free(envp[i]);
			envp[i] = ft_strjoin("PWD=", cwd);
			break ;
		}
	}
	return (0);
}