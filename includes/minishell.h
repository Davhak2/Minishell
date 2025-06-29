#ifndef MINISHELL_H
# define MINISHELL_H

// Main structures and functions for minishell

typedef enum e_shell_mode
{
	INTERACTIVE = 0,
	EXECUTING = 1,
	HEREDOC = 2
}					t_shell_mode;

typedef struct s_shell
{
	int				exit_status;
	e_shell_mode	mode;
}					t_shell;

#endif // MINISHELL_H
