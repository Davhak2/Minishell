/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:14:30 by letto             #+#    #+#             */
/*   Updated: 2025/08/11 21:15:32 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **args, char **envp)
{
	int	i;

	if (args[1])
	{
		printf("env: '%s': No such file or directory\n", args[1]);
		return (127);
	}
	i = 0;
	while (envp[i])
		printf("%s\n", envp[i++]);
	return (0);
}
