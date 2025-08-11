/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:58:19 by letto             #+#    #+#             */
/*   Updated: 2025/08/11 21:16:19 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_env_kv(const char *name, const char *value)
{
	char	*tmp;
	char	*kv;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	kv = ft_strjoin(tmp, value);
	free(tmp);
	return (kv);
}

static int	append_env_var(char ***envp_ref, char *new_var)
{
	char	**old;
	char	**neu;
	int		n;
	int		i;

	old = *envp_ref;
	n = env_len(old);
	neu = malloc(sizeof(char *) * (n + 2));
	if (!neu)
		return (free(new_var), 0);
	i = 0;
	while (i < n)
	{
		neu[i] = old[i];
		i++;
	}
	neu[n] = new_var;
	neu[n + 1] = NULL;
	free(old);
	*envp_ref = neu;
	return (1);
}

static int	find_var_index(char **envp, const char *name)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (matches_name(envp[i], name))
			return (i);
		i++;
	}
	return (-1);
}

int	update_env_value(const char *name, const char *value, t_shell *shell)
{
	char	*new_var;
	int		idx;

	if (!name || !value || !shell || !shell->envp)
		return (0);
	new_var = make_env_kv(name, value);
	if (!new_var)
		return (0);
	idx = find_var_index(*shell->envp, name);
	if (idx >= 0)
		return (replace_in_place(shell->envp, idx, new_var));
	return (append_env_var(shell->envp, new_var));
}
