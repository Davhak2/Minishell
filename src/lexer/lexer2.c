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

int	handle_quotes(t_token **list, char **ptr)
{
	char		*processed;
	t_tokens	qt;
	t_segment	*segments;
	t_token		*token;
	t_token		*current;

	processed = process_quotes(ptr, &qt, &segments);
	if (!processed)
		return (free_token_list(*list), 0);
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
	if (!*list)
		*list = token;
	else
	{
		current = *list;
		while (current->next)
			current = current->next;
		current->next = token;
	}
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

static int	has_mixed_content(char *ptr)
{
	char	*temp;
	int		has_word;
	int		has_quote;

	temp = ptr;
	has_word = 0;
	has_quote = 0;
	while (*temp && !is_whitespace(*temp) && !is_operator_char(*temp))
	{
		if (is_quote(*temp))
			has_quote = 1;
		else if (is_word_char(*temp))
			has_word = 1;
		temp++;
	}
	return (has_word && has_quote);
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

	if (((**ptr == ';') || (**ptr == '\\') || ((**ptr == '&') && (*(*ptr
						+ 1) != '&'))) && !is_quote(**ptr))
		return (syntax_exit(**ptr, *list), 0);
	if ((is_word_char(**ptr) || is_quote(**ptr)) && has_mixed_content(*ptr))
		return (handle_quotes(list, ptr));
	if (is_quote(**ptr))
		return (handle_quotes(list, ptr));
	if (is_operator_char(**ptr))
		return (read_operator(&type, ptr), create_and_add(list, type), 1);
	if (is_word_char(**ptr))
		return (handle_word(list, ptr));
	return (syntax_exit(**ptr, *list), 0);
}
