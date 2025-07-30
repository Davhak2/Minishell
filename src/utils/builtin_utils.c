#include "libft.h"
#include "utils.h"

char	**duplicate_envp(char **envp)
{
	char	**new_envp;
	int		count;
	int		i;

	if (!envp)
		return (NULL);
	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}

void	free_envp(char **envp)
{
	int i;

	if (!envp)
		return ;

	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}
