#include "builtins.h"
#include "libft.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

char	**remove_env_var(char **envp, int index)
{
	int		count;
	char	**new_envp;
	int		i;
	int		j;

	count = count_env_vars(envp);
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return (NULL);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (i != index)
		{
			new_envp[j] = ft_strdup(envp[i]);
			if (!new_envp[j])
			{
				while (--j >= 0)
					free(new_envp[j]);
				free(new_envp);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	return (new_envp);
}

int	ft_unset(char **args, t_shell *shell)
{
	int i;
	int index;
	char **current_envp;
	char **new_envp;

	current_envp = *(shell->envp);
	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			printf("bash: unset: `%s': not a valid identifier\n", args[i]);
			i++;
			continue ;
		}
		index = find_env_var(current_envp, args[i]);
		if (index >= 0)
		{
			new_envp = remove_env_var(current_envp, index);
			if (!new_envp)
				return (1);
			*(shell->envp) = new_envp;
			free_envp(current_envp);
			current_envp = new_envp;
		}
		i++;
	}
	return (0);
}