/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:55:35 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 16:55:35 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_error(char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'\n", 2);
}

void	free_redirects(t_redirect *redir)
{
	t_redirect	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		if (redir->heredoc_filename)
			free(redir->heredoc_filename);
		free(redir);
		redir = tmp;
	}
}

void	free_ast(t_node *node)
{
	t_cmd	*cmd;

	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
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
