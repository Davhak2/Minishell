#include "builtins.h"
#include "libft.h"
#include "utils.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	find_env_var(char **envp, char *var_name)
{
	int	i;
	int	len;

	if (!var_name)
		return (-1);
	len = ft_strlen(var_name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && (envp[i][len] == '='
				|| envp[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static char	**remove_env_var(char **envp, int index)
{
	char	**new_envp;
	int		count;
	int		i;
	int		j;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count)
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
	int		i;
	int		index;
	char	**new_envp;
	int		exit_status;

	if (!args || !shell || !shell->envp)
		return (1);
	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			exit_status = 1;
		}
		else
		{
			index = find_env_var(*(shell->envp), args[i]);
			if (index >= 0)
			{
				new_envp = remove_env_var(*(shell->envp), index);
				if (new_envp)
				{
					free_envp(*(shell->envp));
					*(shell->envp) = new_envp;
				}
				else
					exit_status = 1;
			}
		}
		i++;
	}
	return (exit_status);
}
