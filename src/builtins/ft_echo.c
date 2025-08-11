/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:41:10 by letto             #+#    #+#             */
/*   Updated: 2025/08/11 21:15:38 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_echo_helper(char **args, int i, t_shell *shell)
{
	char	*expanded;

	if (args[i][0] == '~' && shell && shell->envp)
	{
		expanded = expand_tilde(args[i], *(shell->envp));
		printf("%s", expanded);
		free(expanded);
	}
	else
		printf("%s", args[i]);
	if (args[i + 1])
		printf(" ");
}

int	ft_echo(char **args, t_shell *shell)
{
	int		i;
	int		newline;

	i = 1;
	newline = 1;
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (args[i])
	{
		ft_echo_helper(args, i, shell);
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
