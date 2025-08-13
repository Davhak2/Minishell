/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:28:39 by ganersis          #+#    #+#             */
/*   Updated: 2025/08/11 19:28:39 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredocs_if_needed(t_node *node, t_shell *shell,
		int skip_heredocs)
{
	int	heredoc_counter;
	int	heredoc_status;

	if (skip_heredocs)
		return (0);
	heredoc_counter = 0;
	heredoc_status = process_all_heredocs(node, shell, &heredoc_counter);
	if (heredoc_status != 0)
	{
		shell->last_status = heredoc_status;
		return (1);
	}
	return (0);
}

void	execute_ast_internal(t_node *node, t_shell *shell, int skip_heredocs)
{
	t_cmd	*cmd;
	int		status;

	if (!node)
		return ;
	if (handle_heredocs_if_needed(node, shell, skip_heredocs))
		return ;
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		expand_ast(node, *(shell->envp), shell);
		status = execute_command(cmd, shell);
		shell->last_status = status;
	}
	else if (node->type == PIPE)
		execute_pipe(node, shell);
	else if (node->type == AND || node->type == OR)
		execute_logical_ops(node, shell, skip_heredocs);
	else if (node->type == SUBSHELL)
		execute_subshell(node, shell, skip_heredocs);
	if (!skip_heredocs)
		cleanup_heredoc_files(node);
}

void	execute_ast(t_node *node, t_shell *shell)
{
	execute_ast_internal(node, shell, 0);
}
