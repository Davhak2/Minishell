#include "minishell.h"

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

void	sigint_heredoc_handler(int sig) // TODO: fix here-doc CTRL + C
{
	(void)sig;
	g_received_signal = SIGINT;
	rl_done = 1;
}
