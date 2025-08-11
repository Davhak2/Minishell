/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:15:16 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 19:57:02 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	count_args_exit(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

static int	parse_status_or_error(char *s, int *status_out)
{
	long	tmp;

	if (!is_numeric(s))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd((char *)s, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		*status_out = 2;
		return (0);
	}
	tmp = ft_atoi(s);
	*status_out = (unsigned char)tmp;
	return (1);
}

int	ft_exit(char **args, t_shell *shell)
{
	int	cnt;
	int	exit_status;
	int	ret;

	printf("exit\n");
	cnt = count_args_exit(args);
	if (cnt == 1)
		exit_status = shell->last_status;
	else if (cnt == 2)
		parse_status_or_error(args[1], &exit_status);
	else
	{
		if (parse_status_or_error(args[1], &exit_status))
		{
			ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO);
			ret = 1;
		}
		else
			ret = 0;
		if (ret == 1)
			return (1);
	}
	free_shell(shell);
	exit(exit_status);
}
