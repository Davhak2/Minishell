#include "builtins.h"
#include "minishell.h"
#include "parser.h"
#include "utils.h"

int	ft_exit(char **args, t_shell *shell)
{
	free_shell(shell);
	printf("%s\n", "exit");
	exit(0);
}
