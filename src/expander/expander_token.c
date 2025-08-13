/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davihako <davihako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:19:33 by davihako          #+#    #+#             */
/*   Updated: 2025/08/13 11:19:34 by davihako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_segment_content(t_segment *segment, char **envp,
		int last_status)
{
	if (segment->type == SINGLE_QUOTED)
		return (ft_strdup(segment->value));
	else if (segment->type == DOUBLE_QUOTED || segment->type == WORD)
		return (expand_word(segment->value, envp, last_status));
	return (ft_strdup(segment->value));
}

static char	*build_expanded_result(t_segment *segments,
		char **envp, int last_status)
{
	t_segment	*current;
	char		*expanded_part;
	char		*result;
	char		*temp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	current = segments;
	while (current)
	{
		expanded_part = expand_segment_content(current, envp, last_status);
		if (expanded_part)
		{
			temp = ft_strjoin(result, expanded_part);
			free(result);
			free(expanded_part);
			result = temp;
		}
		current = current->next;
	}
	return (result);
}

void	expand_token_segments(t_token *token, char **envp, int last_status)
{
	char	*result;

	if (!token || !token->segments)
		return ;
	result = build_expanded_result(token->segments, envp, last_status);
	if (!result)
		return ;
	if (token->final_value)
		free(token->final_value);
	token->final_value = result;
}

void	expand_all_tokens(t_token *tokens, char **envp, int last_status)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->segments)
			expand_token_segments(current, envp, last_status);
		else if (current->final_value && (current->type == WORD
				|| current->type == DOUBLE_QUOTED))
		{
			expanded = expand_word(current->final_value, envp, last_status);
			if (expanded)
			{
				free(current->final_value);
				current->final_value = expanded;
			}
		}
		current = current->next;
	}
}
