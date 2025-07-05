/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:39:17 by luminous          #+#    #+#             */
/*   Updated: 2025/07/05 16:05:06 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_token	*create_token(t_tokens type, char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->next = NULL;
	token->value = value;
	token->type = type;
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

void	free_token_list(t_token *head)
{
	t_token	*next;

	while (head)
	{
		next = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = next;
	}
}
