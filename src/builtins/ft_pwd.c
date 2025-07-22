#include "builtins.h"
#include "expander.h"

int	ft_pwd(char **envp)
{
	char	*pwd;

	pwd = get_env_value("PWD", envp);
	if (pwd)
	{
		printf("%s\n", pwd);
		free(pwd);
	}
	return (0);
}
