#include "builtins.h"
#include "expander.h"
#include "libft.h"
#include <unistd.h>

int	ft_cd(char **args, char **envp) // TODO: NOT WORKING
{
	char *cwd;

	cwd = ft_strdup("HOME");
	chdir(get_env_value(cwd, envp));
	int i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PWD=", 4) == 0)
		{
			free(envp[i]);
			envp[i] = ft_strjoin("PWD=", cwd);
			break ;
		}
		i++;
	}
	return (0);
}