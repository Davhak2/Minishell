/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:55:35 by letto             #+#    #+#             */
/*   Updated: 2025/08/11 21:16:58 by ganersis         ###   ########.fr       */
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

int	validate_syntax_loop(t_token *curr, t_token *prev, int *paren_depth)
{
	while (curr)
	{
		if (check_lparen_context(prev, curr))
			return (1);
		if (check_empty_paren(curr))
			return (1);
		if (update_paren_depth(curr, paren_depth))
			return (1);
		if (check_binary_sequence(prev, curr))
			return (1);
		if (check_redirect_delimiter(curr))
			return (1);
		if (check_trailing_op_or_redir(curr))
			return (1);
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*curr;
	t_token	*prev;
	int		paren_depth;

	if (!tokens)
		return (0);
	curr = tokens;
	prev = NULL;
	paren_depth = 0;
	if (is_binary_op(curr->type))
		return (syntax_error(curr->final_value), 1);
	if (validate_syntax_loop(curr, prev, &paren_depth))
		return (1);
	if (paren_depth > 0)
		return (syntax_error("newline"), 1);
	return (0);
}
