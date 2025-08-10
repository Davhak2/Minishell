/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:55:46 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 17:05:20 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include "utils.h"

/* TODO: add syntax error when no argument after redirection and "<|" example 
: "echo >>", "echo <", ">>"*/

int	validate_syntax(t_token *tokens)
{
	t_token	*curr;
	t_token	*prev;

	if (!tokens)
		return (0);
	curr = tokens;
	prev = NULL;
	if (curr->type == PIPE || curr->type == AND || curr->type == OR)
		return (syntax_error(curr->value), 1);
	while (curr)
	{
		if ((curr->type == PIPE || curr->type == AND || curr->type == OR)
			&& prev && (prev->type == PIPE || prev->type == AND
				|| prev->type == OR))
			return (syntax_error(curr->value), 1);
		if ((curr->type == PIPE || curr->type == AND || curr->type == OR)
			&& !curr->next)
			return (syntax_error(curr->value), 1);
		prev = curr;
		curr = curr->next;
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
