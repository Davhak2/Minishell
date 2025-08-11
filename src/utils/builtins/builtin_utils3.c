/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:58:21 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 17:58:21 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

int	env_len(char **envp)
{
	int	i;

	if (!envp)
		return (0);
	i = 0;
	while (envp[i])
		i++;
	return (i);
}

int	matches_name(const char *entry, const char *name)
{
	size_t	n;

	if (!entry || !name)
		return (0);
	n = ft_strlen(name);
	return (ft_strncmp(entry, name, n) == 0 && entry[n] == '=');
}

int	replace_in_place(char ***envp_ref, int idx, char *new_var)
{
	if (!envp_ref || !*envp_ref || idx < 0)
		return (free(new_var), 0);
	free((*envp_ref)[idx]);
	(*envp_ref)[idx] = new_var;
	return (1);
}
