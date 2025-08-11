/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:28:48 by ganersis          #+#    #+#             */
/*   Updated: 2025/08/11 19:28:49 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_heredoc_files(t_node *node)
{
	t_cmd		*cmd;
	t_redirect	*current;

	if (!node)
		return ;
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		if (cmd && cmd->redirects)
		{
			current = cmd->redirects;
			while (current)
			{
				if (current->type == REDIRECT_HEREDOC
					&& current->heredoc_filename)
					unlink(current->heredoc_filename);
				current = current->next;
			}
		}
	}
	else if (node->type == PIPE || node->type == AND || node->type == OR)
	{
		cleanup_heredoc_files(node->left);
		cleanup_heredoc_files(node->right);
	}
}
