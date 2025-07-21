#include "builtins.h"
#include "stdio.h"
#include "stdlib.h"

int	ft_exit(char **args)
{
	printf("%s\n", "exit");
	exit(0);
}
