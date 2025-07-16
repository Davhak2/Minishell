#ifndef SIGNALS_H
# define SIGNALS_H

// Signal handling

# include "minishell.h"

void							init_signals(void);
void							sigint_handler(int sig);

#endif // SIGNALS_H
