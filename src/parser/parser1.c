/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:55:46 by letto             #+#    #+#             */
/*   Updated: 2025/08/12 18:13:02 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_paren_depth(t_token *curr, int *paren_depth)
{
	if (curr->type == LPAREN)
		(*paren_depth)++;
	else if (curr->type == RPAREN)
	{
		if (*paren_depth == 0)
			return (syntax_error(")"), 1);
		(*paren_depth)--;
	}
	return (0);
}

int	check_trailing_op_or_redir(t_token *curr)
{
	if (!curr->next)
	{
		if (is_binary_op(curr->type))
			return (syntax_error("newline"), 1);
		if (curr->type == REDIRECT_APPEND || curr->type == REDIRECT_HEREDOC
			|| curr->type == REDIRECT_IN || curr->type == REDIRECT_OUT)
			return (syntax_error("newline"), 1);
	}
	return (0);
}

t_node	*parse(t_token **list)
{
	return (parse_or(list));
}

static t_node	*build_node(char **argv, t_tokens *types, t_redirect *rh)
{
	t_cmd	*cmd;
	t_node	*node;

	cmd = ft_calloc(1, sizeof(*cmd));
	node = ft_calloc(1, sizeof(*node));
	if (!cmd || !node)
	{
		free(cmd);
		free(node);
		free(argv);
		free(types);
		return (free_redirects(rh), NULL);
	}
	if (argv && argv[0])
		cmd->cmd = argv[0];
	else
		cmd->cmd = NULL;
	cmd->args = argv;
	cmd->arg_types = types;
	cmd->redirects = rh;
	node->type = WORD;
	node->value = (char *)cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_node	*simple_command(t_token **list)
{
	t_token		*end;
	t_redirect	*rh;
	int			argc;
	char		**argv;
	t_tokens	*types;

	if (!*list || !in_simple_span((*list)->type))
		return (NULL);
	if (scan_and_collect(*list, &end, &rh) < 0)
		return (NULL);
	argc = count_args(*list);
	if (argc == 0 && !rh)
		return (*list = end, NULL);
	if (alloc_arrays(argc, &argv, &types) < 0)
		return (free_redirects(rh), NULL);
	if (fill_args(*list, argv, types) < 0)
	{
		free(argv);
		free(types);
		free_redirects(rh);
		return (NULL);
	}
	*list = end;
	return (build_node(argv, types, rh));
}
