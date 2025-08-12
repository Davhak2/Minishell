/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:21:16 by letto             #+#    #+#             */
/*   Updated: 2025/08/12 16:03:36 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	if (!str || str[0] == '=')
		return (ft_strdup(str));
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	name = ft_substr(str, 0, i);
	return (name);
}

char	**update_or_add_env_var(char **envp, char *new_var)
{
	int		index;
	char	*var_name;

	var_name = get_var_name(new_var);
	if (!var_name)
		return (NULL);
	index = find_env_var(envp, var_name);
	free(var_name);
	if (index >= 0)
		return (update_env_var(envp, new_var, index));
	return (add_env_var(envp, new_var));
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
	i = -1;
	while (++i < count)
	{
		if (i == index)
			new_envp[i] = ft_strdup(new_var);
		else
			new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			return (free(new_envp), NULL);
		}
	}
	return (new_envp[count] = NULL, new_envp);
}
