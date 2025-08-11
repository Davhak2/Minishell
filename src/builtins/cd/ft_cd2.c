/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:38:38 by letto             #+#    #+#             */
/*   Updated: 2025/08/11 21:14:43 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_oldpwd(t_shell *shell)
{
	char	*oldpwd;

	oldpwd = get_env_value("OLDPWD", *shell->envp);
	if (!oldpwd)
		return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
	if (chdir(oldpwd) == -1)
		return (free(oldpwd), perror("cd"), 1);
	ft_putstr_fd(oldpwd, 1);
	ft_putstr_fd("\n", 1);
	free(oldpwd);
	return (0);
}

char	*cwd_with_fallback(char **args, t_shell *shell)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
		return (cwd);
	pwd = get_env_value("PWD", *shell->envp);
	ft_putstr_fd("cd: error retrieving current directory: getcwd: "
		"cannot access parent directories\n", 2);
	if (args[1])
		cwd = path_join_fallback(pwd, args[1]);
	free(pwd);
	if (!cwd)
	{
		ft_putstr_fd("cd: fallback path construction failed\n", 2);
		return (NULL);
	}
	return (cwd);
}

int	update_pwd_vars(char *cwd, t_shell *shell)
{
	char	*old;

	old = get_env_value("PWD", *shell->envp);
	if (old)
	{
		if (!update_env_value("OLDPWD", old, shell))
			return (free(old), free(cwd),
				ft_putstr_fd("cd: failed to update OLDPWD\n", 2), 1);
		free(old);
	}
	if (!update_env_value("PWD", cwd, shell))
		return (free(cwd),
			ft_putstr_fd("cd: failed to update PWD\n", 2), 1);
	free(cwd);
	return (0);
}

int	cd_home(t_shell *shell)
{
	char	*home;

	home = get_env_value("HOME", *shell->envp);
	if (!home)
		return (ft_putstr_fd("cd: HOME not set\n", 2), 1);
	if (chdir(home) == -1)
		return (free(home), perror("cd"), 1);
	free(home);
	return (0);
}
