#include "signals.h"

void init_signals()
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler); // TODO: fix CTRL + \ ""
}
