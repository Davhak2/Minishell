/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:55:42 by letto             #+#    #+#             */
/*   Updated: 2025/08/12 15:54:41 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_redirect(t_redirect **rh, t_redirect **last, t_token **cur)
{
	t_redirect	*r;

	r = ft_calloc(1, sizeof(*r));
	if (!r)
		return (free_redirects(*rh), -1);
	r->type = (*cur)->type;
	*cur = (*cur)->next;
	if (!*cur || !is_arg_token((*cur)->type))
		return (free_redirects(*rh), -1);
	r->filename = ft_strdup((*cur)->final_value);
	r->next = NULL;
	if (*rh == NULL)
	{
		*rh = r;
		*last = r;
	}
	else
	{
		(*last)->next = r;
		*last = r;
	}
	*cur = (*cur)->next;
	return (0);
}

int	scan_and_collect(t_token *cur, t_token **end, t_redirect **rh)
{
	t_redirect	*last;

	*rh = NULL;
	last = NULL;
	while (cur && in_simple_span(cur->type))
	{
		if (is_arg_token(cur->type))
			cur = cur->next;
		else if (add_redirect(rh, &last, &cur) < 0)
			return (-1);
	}
	*end = cur;
	return (0);
}

int	fill_args(t_token *cur, char **argv, t_tokens *types)
{
	int	i;

	i = 0;
	while (cur && in_simple_span(cur->type))
	{
		if (is_arg_token(cur->type))
		{
			argv[i] = ft_strdup(cur->final_value);
			if (!argv[i])
				return (-1);
			types[i++] = cur->type;
		}
		else
			cur = cur->next;
		if (cur)
			cur = cur->next;
		else
			cur = NULL;
	}
	argv[i] = NULL;
	return (0);
}

int	count_args(t_token *cur)
{
	int	n;

	n = 0;
	while (cur && in_simple_span(cur->type))
	{
		if (is_arg_token(cur->type))
			n++;
		else
		{
			if (cur->next)
				cur = cur->next;
		}
		if (cur)
			cur = cur->next;
		else
			cur = NULL;
	}
	return (n);
}

int	alloc_arrays(int argc, char ***argv, t_tokens **types)
{
	*argv = ft_calloc((size_t)argc + 1, sizeof(char *));
	*types = malloc(sizeof(t_tokens) * (size_t)argc);
	if (!*argv || !*types)
	{
		free(*argv);
		free(*types);
		return (-1);
	}
	return (0);
}
