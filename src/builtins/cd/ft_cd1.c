/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:40:17 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 17:40:17 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*path_join_fallback(char *base, char *rel)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(base, "/");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, rel);
	free(tmp);
	if (!result)
		return (NULL);
	return (result);
}

static int	cd_path(const char *path)
{
	if (chdir(path) == -1)
		return (perror("cd"), 1);
	return (0);
}

int	ft_cd(char **args, t_shell *shell)
{
	int		rc;
	char	*cwd;

	if (!args[1] || !ft_strcmp(args[1], "~"))
		rc = cd_home(shell);
	else if (!ft_strcmp(args[1], "-"))
		rc = cd_oldpwd(shell);
	else
		rc = cd_path(args[1]);
	if (rc != 0)
		return (rc);
	cwd = cwd_with_fallback(args, shell);
	if (!cwd)
		return (1);
	return (update_pwd_vars(cwd, shell));
}
