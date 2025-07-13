#include "signals.h"

void init_signals()
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN); // TODO: fix CTRL + \ ""
}
