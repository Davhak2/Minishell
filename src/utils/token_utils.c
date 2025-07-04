/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luminous <luminous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:39:17 by luminous          #+#    #+#             */
/*   Updated: 2025/07/04 18:50:34 by luminous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_token *create_token(t_tokens type, char *value)
{
	t_token *token;

	token = &(t_token){.type = type, .value = value, .next = NULL};
}

void add_token_to_list(t_token **head, t_token *new_token)
{
	t_token *last;

	last = *head;
	while (last->next)
		last = last->next;
	last->next = new_token;
}

void free_token_list(t_token *head)
{
	t_token *next = head->next;

	while (head->next)
	{
		free(head);
		head = next;
		next = next->next;
	}
	free(head);
}
