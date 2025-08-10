/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:00:45 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 18:01:06 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"
#include "parser.h"
#include "utils.h"

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->redir)
		free_redirects(shell->redir);
	if (shell->token)
		free_token_list(shell->token);
	if (shell->node)
		free_ast(shell->node);
	if (shell->envp && *(shell->envp))
		free_envp(*(shell->envp));
	if (shell->stdin_backup >= 0)
		close(shell->stdin_backup);
	if (shell->stdout_backup >= 0)
		close(shell->stdout_backup);
	free(shell);
}
