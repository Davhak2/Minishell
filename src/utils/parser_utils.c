/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:16:52 by ganersis          #+#    #+#             */
/*   Updated: 2025/08/11 21:16:52 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirects(t_redirect *redir)
{
	t_redirect *tmp;
	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		if (redir->heredoc_filename)
		{
			free(redir->heredoc_filename);
		}
		free(redir);
		redir = tmp;
	}
}

void	free_ast(t_node *node)
{
	if (!node)
		return;
	free_ast(node->left);
	free_ast(node->right);
	if (node->type == WORD && node->value)
	{
		t_cmd *cmd = (t_cmd *)node->value;
		if (cmd->args)
			ft_free_array(cmd->args);
		if (cmd->arg_types)
			free(cmd->arg_types);
		if (cmd->redirects)
			free_redirects(cmd->redirects);
		free(cmd);
	}
	free(node);
}
