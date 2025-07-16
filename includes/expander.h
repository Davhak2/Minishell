#ifndef EXPANDER_H
# define EXPANDER_H

# include "parser.h"

void	expand_ast(t_node *node, char **envp, int last_status);

#endif