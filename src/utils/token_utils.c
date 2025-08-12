/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:39:17 by luminous          #+#    #+#             */
/*   Updated: 2025/08/12 15:55:12 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_tokens type, t_segment *segments)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->final_value = NULL;
	token->segments = segments;
	token->next = NULL;
	return (token);
}

void	add_token_to_list(t_token **head, t_token *new_token)
{
	t_token	*last;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new_token;
}

static void	free_segments(t_segment *segments)
{
	t_segment	*next;

	while (segments)
	{
		next = segments->next;
		if (segments->value)
			free(segments->value);
		free(segments);
		segments = next;
	}
}

void	free_token_list(t_token *head)
{
	t_token	*next;

	while (head)
	{
		next = head->next;
		if (head->final_value)
			free(head->final_value);
		if (head->segments)
			free_segments(head->segments);
		free(head);
		head = next;
	}
}
