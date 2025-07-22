#include "builtins.h"
#include "libft.h"
#include "utils.h"
#include <stdio.h>

int	is_valid_identifier(char *str)
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

int	has_special_chars(char *str)
{
	int		i;
	char	*special;

	special = "(){}[]<>|&;$`\"'*?~";
	i = 0;
	while (str[i])
	{
		if (ft_strchr(special, str[i]))
			return (i);
		i++;
	}
	return (-1);
}

char	*get_var_name(char *str)
{
	int		i;
	char	*name;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	name = ft_substr(str, 0, i);
	return (name);
}

int	find_env_var(char **envp, char *var_name)
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

int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

char	**update_env_var(char **envp, char *new_var, int index)
{
	char	**new_envp;
	int		count;
	int		i;

	if (!envp || !new_var)
		return (NULL);
	count = count_env_vars(envp);
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if (i == index)
			new_envp[i] = ft_strdup(new_var);
		else
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
char	**add_env_var(char **envp, char *new_var)
{
	int		count;
	char	**new_envp;
	int		i;

	count = count_env_vars(envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
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
	new_envp[i] = ft_strdup(new_var);
	if (!new_envp[i])
	{
		while (--i >= 0)
			free(new_envp[i]);
		free(new_envp);
		return (NULL);
	}
	new_envp[i + 1] = NULL;
	return (new_envp);
}

char	**update_or_add_env_var(char **envp, char *new_var)
{
	int		index;
	char	*var_name;
	char	**new_envp;

	var_name = get_var_name(new_var);
	if (!var_name)
		return (NULL);
	index = find_env_var(envp, var_name);
	free(var_name);
	if (index >= 0)
		return (update_env_var(envp, new_var, index));
	return (add_env_var(envp, new_var));
}

char	*create_second_var(char *str)
{
	char	*first_equals;
	char	*second_equals;
	char	*var_name;
	char	*result;

	first_equals = ft_strchr(str, '=');
	if (!first_equals)
		return (NULL);
	second_equals = ft_strchr(first_equals + 1, '=');
	if (!second_equals)
		return (NULL);
	var_name = ft_substr(str, first_equals - str + 1, second_equals
			- first_equals - 1);
	if (!var_name)
		return (NULL);
	result = ft_strjoin(var_name, second_equals);
	free(var_name);
	return (result);
}

int	is_valid_second_var(char *str)
{
	char	*var_name;
	int		result;

	if (!str)
		return (0);
	var_name = get_var_name(str);
	if (!var_name)
		return (0);
	result = is_valid_identifier(var_name);
	free(var_name);
	return (result);
}
int	ft_export(char **args, t_shell *shell)
{
	int i;
	char *var_name;
	char **current_envp;
	char **new_envp;
	char *equals_pos;

	current_envp = *(shell->envp);
	if (!args[1])
	{
		i = 0;
		while (current_envp[i])
		{
			printf("declare -x ");
			equals_pos = ft_strchr(current_envp[i], '=');
			if (equals_pos)
			{
				*equals_pos = '\0';
				printf("%s=\"%s\"\n", current_envp[i], equals_pos + 1);
				*equals_pos = '=';
			}
			else
				printf("%s\n", current_envp[i]);
			i++;
		}
		return (0);
	}
	i = 1;
	while (args[i])
	{
		var_name = get_var_name(args[i]);
		if (!var_name)
		{
			i++;
			continue ;
		}
		if (!is_valid_identifier(var_name))
		{
			printf("bash: export: `%s': not a valid identifier\n", var_name);
			free(var_name);
			i++;
			continue ;
		}
		equals_pos = ft_strchr(args[i], '=');
		if (equals_pos)
		{
			new_envp = update_or_add_env_var(current_envp, args[i]);
			if (!new_envp)
			{
				free(var_name);
				return (1);
			}
			*(shell->envp) = new_envp;
			free_envp(current_envp);
			current_envp = new_envp;
		}
		else
		{
			new_envp = add_env_var(current_envp, var_name);
			if (!new_envp)
			{
				free(var_name);
				return (1);
			}
			*(shell->envp) = new_envp;
			free_envp(current_envp);
			current_envp = new_envp;
		}
		free(var_name);
		i++;
	}
	return (0);
}