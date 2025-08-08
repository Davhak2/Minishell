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
	g_received_signal = SIGINT;

	// Write newline to move cursor to next line
	write(STDOUT_FILENO, "\n", 1);

	// Clear the line and force readline to return
	rl_replace_line("", 0);
	rl_point = 0;
	rl_end = 0;
	rl_done = 1;
}
