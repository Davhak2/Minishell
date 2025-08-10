/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:30:57 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 15:35:00 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "parser.h"

typedef struct s_redirect_state
{
	int	pipefd[2];
	int	has_pipe;
}		t_redirect_state;

int		execute_command(t_cmd *cmd, t_shell *shell);
void	execute_ast(t_node *node, t_shell *shell);
int		handle_redirects(t_redirect *redirects, t_redirect_state *state,
			t_shell *shell);
#endif // EXECUTOR_H
