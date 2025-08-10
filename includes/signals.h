/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:31:12 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 17:05:41 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"
# include <sys/ioctl.h>

extern volatile sig_atomic_t	g_received_signal;

void							init_signals(void);
void							setup_heredoc_signals(void);
void							setup_signals_parent_exec(void);
void							setup_signals(void);
void							restore_signals(void);
void							sigint_handler(int sig);
void							sigint_heredoc_handler(int sig);

#endif // SIGNALS_H
