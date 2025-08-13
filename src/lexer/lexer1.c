/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davihako <davihako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:20:12 by davihako          #+#    #+#             */
/*   Updated: 2025/08/13 11:20:13 by davihako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_unquoted(char **ptr, t_segment **segments,
		char **result, int *has_segments)
{
	char		*start;
	char		*temp_value;
	t_segment	*segment;

	start = *ptr;
	while (**ptr && !is_whitespace(**ptr) && !is_quote(**ptr)
		&& !is_operator_char(**ptr))
		(*ptr)++;
	if (*ptr <= start)
		return (0);
	temp_value = ft_substr(start, 0, *ptr - start);
	if (!temp_value)
		return (free(*result), -1);
	segment = create_segment(WORD, temp_value);
	if (!segment)
		return (free(*result), free(temp_value), -1);
	add_segment(segments, segment);
	*has_segments = 1;
	if (append_slice(result, temp_value, ft_strlen(temp_value)) < 0)
		return (free(temp_value), -1);
	free(temp_value);
	return (0);
}

static int	set_quote_type(t_tokens *quote_type, char qc)
{
	if (*quote_type == WORD)
	{
		if (qc == '\'')
			*quote_type = SINGLE_QUOTED;
		else
			*quote_type = DOUBLE_QUOTED;
	}
	return (0);
}

char	*process_quotes(char **ptr, t_tokens *quote_type,
		t_segment **segments)
{
	char	*result;
	int		has_segments;
	char	qc;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	has_segments = 0;
	*quote_type = WORD;
	*segments = NULL;
	while (**ptr && !is_whitespace(**ptr) && !is_operator_char(**ptr))
	{
		if (is_quote(**ptr))
		{
			qc = **ptr;
			set_quote_type(quote_type, qc);
			if (handle_quoted(ptr, segments, &result, &has_segments) < 0)
				return (NULL);
		}
		else if (handle_unquoted(ptr, segments, &result, &has_segments) < 0)
			return (NULL);
	}
	if (has_segments)
		return (free(result), ft_strdup(""));
	return (result);
}

static void	skip_spaces(char **ptr)
{
	while (**ptr && is_whitespace(**ptr))
		(*ptr)++;
}

t_token	*tokenize(char *line)
{
	t_token	*list;
	char	*ptr;

	list = NULL;
	ptr = line;
	while (*ptr)
	{
		skip_spaces(&ptr);
		if (!*ptr)
			break ;
		if (!dispatch_token(&list, &ptr))
			return (NULL);
	}
	return (list);
}
