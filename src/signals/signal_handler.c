#include "signals.h"

volatile sig_atomic_t	g_received_signal = 0;

void	sigint_handler(int sig)
{
	g_received_signal = sig;
	write (1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	g_received_signal = SIGINT;
}
