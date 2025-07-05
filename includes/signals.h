#ifndef SIGNALS_H
# define SIGNALS_H

// Signal handling

# include "minishell.h"

extern volatile sig_atomic_t	g_received_signal;

void							init_signals(void);
void							sigint_handler(int sig);
void							sigquit_handler(int sig);

#endif // SIGNALS_H
