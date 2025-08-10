/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 20:09:45 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 20:13:47 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "utils.h"

static int	handle_name_only(char *name, char ***cur_ref, t_shell *shell)
{
	char	**neu;

	neu = add_env_var(*cur_ref, name);
	if (!neu)
		return (1);
	*(shell->envp) = neu;
	free_envp(*cur_ref);
	*cur_ref = neu;
	return (0);
}

static int	validate_or_error(char *name)
{
	if (is_valid_identifier(name))
		return (1);
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (0);
}

static int	process_one_export(char *arg, char ***cur_ref, t_shell *shell)
{
	char	*name;
	char	*eq;
	int		rc;

	name = get_var_name(arg);
	if (!name)
		return (0);
	if (!validate_or_error(name))
		return (free(name), 1);
	eq = ft_strchr(arg, '=');
	if (eq)
		rc = handle_assignment(arg, cur_ref, shell);
	else
		rc = handle_name_only(name, cur_ref, shell);
	free(name);
	return (rc);
}

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	char	**cur;
	int		ret;

	cur = *(shell->envp);
	if (!args[1])
		return (print_declared(cur), 0);
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (process_one_export(args[i], &cur, shell))
			return (1);
		i++;
	}
	return (ret);
}
