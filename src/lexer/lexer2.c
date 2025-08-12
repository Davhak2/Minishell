/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:17:50 by letto             #+#    #+#             */
/*   Updated: 2025/08/12 16:29:04 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_token_from_quotes(t_token **list, t_tokens qt,
		t_segment *segments, char *processed)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (free_token_list(*list), 0);
	token->type = qt;
	if (segments)
	{
		token->final_value = NULL;
		token->segments = segments;
	}
	else
	{
		token->final_value = processed;
		token->segments = NULL;
	}
	token->next = NULL;
	add_token_to_list(list, token);
	return (1);
}

int	handle_quotes(t_token **list, char **ptr)
{
	char		*processed;
	t_tokens	qt;
	t_segment	*segments;

	processed = process_quotes(ptr, &qt, &segments);
	if (!processed)
		return (free_token_list(*list), 0);
	if (!create_token_from_quotes(list, qt, segments, processed))
		return (0);
	if (segments)
		free(processed);
	return (1);
}

int	read_operator(t_type *type, char **ptr)
{
	char	c;

	c = **ptr;
	if ((c == '<' && *(*ptr + 1) == '<') || (c == '>' && *(*ptr + 1) == '>')
		|| (c == '|' && *(*ptr + 1) == '|') || (c == '&' && *(*ptr + 1) == '&'))
	{
		*type = set_type(c, 0);
		*ptr += 2;
	}
	else
	{
		*type = set_type(c, 1);
		(*ptr)++;
	}
	return (1);
}

int	handle_word(t_token **list, char **ptr)
{
	const char	*start;
	size_t		len;
	char		*word;
	t_type		type;

	start = *ptr;
	while (**ptr && is_word_char(**ptr))
		(*ptr)++;
	len = (size_t)(*ptr - start);
	word = malloc(len + 1);
	if (!word)
		return (free_token_list(*list), 0);
	ft_strlcpy(word, start, len + 1);
	type.type = WORD;
	type.value = word;
	create_and_add(list, type);
	return (1);
}

int	dispatch_token(t_token **list, char **ptr)
{
	t_type	type;
	int		has_word;
	int		has_quote;
	int		has_single_amp;
	int		is_forbidden;

	has_single_amp = ((**ptr == '&') && (*(*ptr + 1) != '&'));
	is_forbidden = ((**ptr == ';') || (**ptr == '\\') || has_single_amp);
	if (is_forbidden && !is_quote(**ptr))
		return (syntax_exit(**ptr, *list), 0);
	scan_mixed_lookahead(*ptr, &has_word, &has_quote);
	if ((is_word_char(**ptr) || is_quote(**ptr)) && (has_word && has_quote))
		return (handle_quotes(list, ptr));
	if (is_quote(**ptr))
		return (handle_quotes(list, ptr));
	if (is_operator_char(**ptr))
		return (read_operator(&type, ptr), create_and_add(list, type), 1);
	if (is_word_char(**ptr))
		return (handle_word(list, ptr));
	return (syntax_exit(**ptr, *list), 0);
}
