#include "minishell.h"

int main(int argc, char **argv, char **envp) {

	init_signals();

    while(!0)
	{
		char *tmp = readline("\001\033[1;32m\002💚 🐚 niGGol ➜\001\033[0m\002 ");

		free(tmp);
	}
    return 0;
}
