#include "builtins.h"
#include "expander.h"

int	ft_pwd(char **envp)
{
	printf("\n%s", get_env_value("PWD", envp));
	return (0);
}
