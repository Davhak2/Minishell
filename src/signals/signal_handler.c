#include "../../includes/signals.h"

volatile sig_atomic_t	g_received_signal = 0;

void	sigint_handler(int sig)
{
	g_received_signal = sig;
}

void	sigquit_handler(int sig)
{
	g_received_signal = sig;
}
