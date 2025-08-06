#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "parser.h"

typedef struct s_redirect_state
{
    int pipefd[2];
    int has_pipe;
} t_redirect_state;

int		execute_command(t_cmd *cmd, t_shell *shell);
void	execute_ast(t_node *node, t_shell *shell);
#endif // EXECUTOR_H
