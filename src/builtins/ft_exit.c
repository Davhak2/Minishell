/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:15:16 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 17:20:15 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "parser.h"
#include "utils.h"

static int	is_numeric(char *str)
{
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	ft_exit(char **args, t_shell *shell)
{
	int		exit_status;
	long	tmp;
	int		cnt;

	printf("exit\n");
	cnt = 0;
	while (args[cnt])
		cnt++;
	if (cnt == 1)
		exit_status = shell->last_status;
	else if (cnt == 2)
	{
		if (!is_numeric(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit_status = 2;
		}
		else
		{
			tmp = ft_atoi(args[1]);
			exit_status = (unsigned char)tmp;
		}
	}
	else
	{
		if (!is_numeric(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit_status = 2;
		}
		else
		{
			ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO);
			return (1);
		}
	}
	free_shell(shell);
	exit(exit_status);
}
