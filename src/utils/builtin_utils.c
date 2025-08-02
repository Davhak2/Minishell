#include "libft.h"
#include "utils.h"
#include <stdlib.h>

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

int	update_env_value(const char *name, const char *value, t_shell *shell)
{
	char	*new_var;
	char	*temp;
	char	**new_envp;
	int		i;

	if (!name || !value || !shell || !shell->envp)
		return (0);
	temp = ft_strjoin(name, "=");
	if (!temp)
		return (0);
	new_var = ft_strjoin(temp, value);
	free(temp);
	if (!new_var)
		return (0);
	i = 0;
	while ((*shell->envp)[i])
	{
		if (ft_strncmp((*shell->envp)[i], name, ft_strlen(name)) == 0
			&& (*shell->envp)[i][ft_strlen(name)] == '=')
		{
			free((*shell->envp)[i]);
			(*shell->envp)[i] = new_var;
			return (1);
		}
		i++;
	}
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		return (free(new_var), 0);
	i = 0;
	while ((*shell->envp)[i])
	{
		new_envp[i] = (*shell->envp)[i];
		i++;
	}
	new_envp[i] = new_var;
	new_envp[i + 1] = NULL;
	free(*shell->envp);
	*shell->envp = new_envp;
	return (1);
}
