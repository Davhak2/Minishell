/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:21:49 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 20:15:49 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "utils.h"
#include <stdio.h>

static char	**dup_env_with_capacity(char **envp, int *count_out)
{
	int		count;
	char	**neu;
	int		i;

	count = count_env_vars(envp);
	neu = malloc(sizeof(char *) * (count + 2));
	if (!neu)
		return (NULL);
	i = 0;
	while (i < count)
	{
		neu[i] = ft_strdup(envp[i]);
		if (!neu[i])
		{
			while (--i >= 0)
				free(neu[i]);
			free(neu);
			return (NULL);
		}
		i++;
	}
	neu[count] = NULL;
	neu[count + 1] = NULL;
	*count_out = count;
	return (neu);
}

char	**add_env_var(char **envp, char *new_var)
{
	char	**neu;
	int		count;
	int		i;

	neu = dup_env_with_capacity(envp, &count);
	if (!neu)
		return (NULL);
	neu[count] = ft_strdup(new_var);
	if (!neu[count])
	{
		i = count;
		while (--i >= 0)
			free(neu[i]);
		free(neu);
		return (NULL);
	}
	neu[count + 1] = NULL;
	return (neu);
}

// TODO: export with quotes is valid always, export test = "something" is always valid
void	print_declared(char **envp)
{
	int		i;
	char	*eq;

	i = 0;
	while (envp && envp[i])
	{
		printf("declare -x ");
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			*eq = '\0';
			printf("%s=\"%s\"\n", envp[i], eq + 1);
			*eq = '=';
		}
		else
			printf("%s\n", envp[i]);
		i++;
	}
}

int	handle_assignment(char *arg, char ***cur_ref, t_shell *shell)
{
	char	**neu;

	neu = update_or_add_env_var(*cur_ref, arg);
	if (!neu)
		return (1);
	*(shell->envp) = neu;
	free_envp(*cur_ref);
	*cur_ref = neu;
	return (0);
}
