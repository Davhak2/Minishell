/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 01:14:44 by luminous          #+#    #+#             */
/*   Updated: 2025/08/10 17:00:01 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"
#include "lexer.h"

static int	read_plain(char **p, char **res)
{
	char	*start;

	start = *p;
	while (**p && !is_whitespace(**p) && !is_quote(**p))
		(*p)++;
	if (append_slice(res, start, (size_t)(*p - start)) < 0)
		return (-1);
	return (0);
}

char	*process_quotes(char **ptr, t_tokens *quote_type)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	*quote_type = WORD;
	while (**ptr)
	{
		if (is_quote(**ptr))
		{
			if (read_quoted(ptr, quote_type, &result) < 0)
				return (NULL);
		}
		else if (!is_whitespace(**ptr))
		{
			if (read_plain(ptr, &result) < 0)
				return (NULL);
		}
		else
			break ;
	}
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
