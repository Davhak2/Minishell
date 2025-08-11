/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:29:16 by ganersis          #+#    #+#             */
/*   Updated: 2025/08/11 21:08:26 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int signo)
{
	(void)signo;
	g_received_signal = SIGINT;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	g_received_signal = SIGINT;
	rl_done = 1;
}
