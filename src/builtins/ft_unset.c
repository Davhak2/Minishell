/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:20:25 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 18:12:07 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void	free_partial(char **arr, int upto)
{
	while (--upto >= 0)
		free(arr[upto]);
	free(arr);
}

static char	**dup_env_without(char **envp, int index)
{
	int		n;
	char	**neu;
	int		i;
	int		j;

	n = env_len(envp);
	neu = malloc(sizeof(char *) * n);
	if (!neu)
		return (NULL);
	i = -1;
	j = 0;
	while (envp[++i])
	{
		if (i == index)
			continue ;
		neu[j] = ft_strdup(envp[i]);
		if (!neu[j])
			return (free_partial(neu, j), NULL);
		j++;
	}
	neu[j] = NULL;
	return (neu);
}

static int	unset_one(char *name, t_shell *shell)
{
	int		idx;
	char	**old;
	char	**neu;

	if (!is_valid_identifier(name))
	{
		printf("bash: unset: `%s': not a valid identifier\n", name);
		return (0);
	}
	old = *(shell->envp);
	idx = find_env_var(old, name);
	if (idx < 0)
		return (0);
	neu = dup_env_without(old, idx);
	if (!neu)
		return (1);
	*(shell->envp) = neu;
	free_envp(old);
	return (0);
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	rc;
	int	ret;

	if (!args || !shell || !shell->envp)
		return (1);
	i = 1;
	ret = 0;
	while (args[i])
	{
		rc = unset_one(args[i], shell);
		if (rc != 0)
			ret = rc;
		i++;
	}
	return (ret);
}
