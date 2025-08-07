#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"
#include <sys/ioctl.h>

extern volatile sig_atomic_t	g_received_signal;

void							init_signals(void); // TODO: fix CTRL+C while running command, example: sleep 5 then CTRL+C
void							setup_heredoc_signals(void); // TODO: fix heredoc CTRL+C
void							restore_signals(void);
void							sigint_handler(int sig);
void							sigint_heredoc_handler(int sig);

#endif // SIGNALS_H
