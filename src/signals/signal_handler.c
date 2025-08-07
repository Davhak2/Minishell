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
	struct termios term;

	(void)sig;
	g_received_signal = SIGINT;
	rl_replace_line("", 0);

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);

	ioctl(STDIN_FILENO, TIOCSTI, "\n");

	term.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
