/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 01:14:44 by luminous          #+#    #+#             */
/*   Updated: 2025/08/10 15:16:28 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"

char	*process_quotes(char **ptr, t_tokens *quote_type)
{
	char	*result;
	char	*temp;
	char	quote;
	char	*start;
	char	*joined;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	*quote_type = WORD;
	while (**ptr)
	{
		if (is_quote(**ptr))
		{
			quote = **ptr;
			if (*quote_type == WORD)
			{
				if (quote == '\'')
					*quote_type = SINGLE_QUOTED;
				else if (quote == '"')
					*quote_type = DOUBLE_QUOTED;
			}
			(*ptr)++;
			start = *ptr;
			while (**ptr && **ptr != quote)
				(*ptr)++;
			if (**ptr == quote)
			{
				temp = ft_substr(start, 0, *ptr - start);
				if (!temp)
				{
					free(result);
					return (NULL);
				}
				joined = ft_strjoin(result, temp);
				free(temp);
				free(result);
				result = joined;
				(*ptr)++;
			}
			else
			{
				printf("minishell: syntax error: unclosed quote\n");
				free(result);
				return (NULL);
			}
		}
		else if (!is_whitespace(**ptr) && **ptr != '\0')
		{
			start = *ptr;
			while (**ptr && !is_whitespace(**ptr) && !is_quote(**ptr))
				(*ptr)++;
			temp = ft_substr(start, 0, *ptr - start);
			if (!temp)
			{
				free(result);
				return (NULL);
			}
			joined = ft_strjoin(result, temp);
			free(temp);
			free(result);
			result = joined;
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
