#include "builtins.h"
#include "expander.h"

int	ft_pwd(char **envp)
{
	printf("%s\n", get_env_value("PWD", envp));
	return (0);
}
