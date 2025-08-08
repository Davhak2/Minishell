#include "signals.h"

volatile sig_atomic_t	g_received_signal = 0;

void	sigint_handler(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_received_signal = SIGINT;
}

#include <termios.h>

void sigint_heredoc_handler(int sig)
{
	(void)sig;
	// This signal handler is not used with the fork approach
	// The child process uses SIG_DFL for proper signal handling
	g_received_signal = SIGINT;
}
