/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:55:50 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 18:00:08 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_or(t_token **list)
{
	t_node	*left;
	t_node	*right;
	t_node	*node;

	left = parse_and(list);
	while (*list && (*list)->type == OR)
	{
		*list = (*list)->next;
		right = parse_and(list);
		if (!right)
			return (free_ast(left), NULL);
		node = malloc(sizeof(t_node));
		if (!node)
			return (free_ast(left), free_ast(right), NULL);
		node->type = OR;
		node->left = left;
		node->right = right;
		node->value = NULL;
		left = node;
	}
	return (left);
}

t_node	*parse_and(t_token **list)
{
	t_node	*left;
	t_node	*right;
	t_node	*node;

	left = parse_pipe(list);
	while (*list && (*list)->type == AND)
	{
		*list = (*list)->next;
		right = parse_pipe(list);
		if (!right)
			return (free_ast(left), NULL);
		node = malloc(sizeof(t_node));
		if (!node)
			return (free_ast(left), free_ast(right), NULL);
		node->type = AND;
		node->left = left;
		node->right = right;
		node->value = NULL;
		left = node;
	}
	return (left);
}

t_node	*parse_pipe(t_token **list)
{
	t_node	*left;
	t_node	*right;
	t_node	*node;

	left = parse_parenthesis(list);
	while (*list && (*list)->type == PIPE)
	{
		*list = (*list)->next;
		right = parse_parenthesis(list);
		if (!right)
			return (free_ast(left), NULL);
		node = malloc(sizeof(t_node));
		if (!node)
			return (free_ast(left), free_ast(right), NULL);
		node->type = PIPE;
		node->left = left;
		node->right = right;
		node->value = NULL;
		left = node;
	}
	return (left);
}

t_node	*parse_parenthesis(t_token **list)
{
	t_node	*node;

	if (*list && (*list)->type == LPAREN)
	{
		*list = (*list)->next;
		node = parse_or(list);
		if (!node)
			return (NULL);
		if (!(*list) || (*list)->type != RPAREN)
		{
			free_ast(node);
			return (NULL);
		}
		*list = (*list)->next;
		return (node);
	}
	return (simple_command(list));
}
