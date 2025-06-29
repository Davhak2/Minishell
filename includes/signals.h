#ifndef SIGNALS_H
# define SIGNALS_H

// Signal handling

# include <signal.h>
# include <sys/types.h>

extern volatile sig_atomic_t	g_received_signal;

void							sigint_handler(int sig);
void							sigquit_handler(int sig);

#endif // SIGNALS_H
