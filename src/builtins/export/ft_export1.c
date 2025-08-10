/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:21:49 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 18:23:24 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "utils.h"
#include <stdio.h>

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

int	ft_export(char **args, t_shell *shell)
// TODO: export with quotes is valid always, export test = "something" is always valid
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
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(var_name, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			free(var_name);
			return (1);
			// i++;
			// continue ;
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
