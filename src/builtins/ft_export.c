#include "builtins.h"
#include "libft.h"
#include <stdio.h>

int	ft_export(char **args, char **envp)
{
	int		i;
	int		j;
	int		count;
	char	*tmp;

	if (!args[1])
	{
		i = 0;
		while (envp[i])
		{
			j = 0;
			printf("declare -x ");
			if (ft_strchr(envp[i], '='))
			{
				while (envp[i][j] != '=')
					printf("%c", envp[i][j++]);
				printf("=\"");
				j++;
				while (envp[i][j])
					printf("%c", envp[i][j++]);
				printf("\"\n");
			}
			else
				printf("%s", envp[i]);
			++i;
		}
		return (0);
	}
	else // TODO: handle export with arguments
	{
		count = 0;
		while (envp[count])
			count++;
		i = 1;
		while(args[i])
		{
			tmp = ft_strchr(args[i],'='); 
			if (tmp)
			{

			}
			else
			{

			}
		}
	}
	return (0);
}
