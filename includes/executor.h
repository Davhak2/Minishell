#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "parser.h"

int		execute_command(t_cmd *cmd, char **envp);
void	execute_ast(t_node *node, char **envp, int last_status, t_shell *shell);

#endif // EXECUTOR_H
