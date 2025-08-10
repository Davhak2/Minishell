/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:14:22 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 17:14:23 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "expander.h"

int	ft_pwd(char **envp)
{
	char	*pwd;

	pwd = get_env_value("PWD", envp);
	if (pwd)
	{
		printf("%s\n", pwd);
		free(pwd);
	}
	return (0);
}
