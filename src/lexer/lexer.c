/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 01:14:44 by luminous          #+#    #+#             */
/*   Updated: 2025/07/31 18:50:59 by ganersis         ###   ########.fr       */
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

t_token	*tokenize(char *line)
{
	t_token		*list;
	char		*ptr;
	t_type		type;
	char		*start;
	int			len;
	char		*word;
	char		quote;
	char		*processed;
	t_tokens	quote_type;

	list = NULL;
	ptr = line;
	while (*ptr)
	{
		while (is_whitespace(*ptr))
			++ptr;
		if (!*ptr)
			break ;
		if ((*ptr == ';' || *ptr == '\\') && !is_quote(*ptr))
		{
			printf("minishell: syntax error near unexpected token `%c'\n",
				*ptr);
			free_token_list(list);
			return (NULL);
		}
		if (is_quote(*ptr))
		{
			processed = process_quotes(&ptr, &quote_type);
			if (!processed)
			{
				free_token_list(list);
				return (NULL);
			}
			type.type = quote_type;
			type.value = processed;
			create_and_add(&list, type);
		}
		else if (is_operator_char(*ptr))
		{
			if ((*ptr == '<' && *(ptr + 1) == '<') || (*ptr == '>' && *(ptr
						+ 1) == '>') || (*ptr == '|' && *(ptr + 1) == '|')
				|| (*ptr == '&' && *(ptr + 1) == '&'))
			{
				type = set_type(*ptr, 0);
				ptr += 2;
			}
			else
			{
				type = set_type(*ptr, 1);
				ptr++;
			}
			create_and_add(&list, type);
		}
		else if (is_word_char(*ptr))
		{
			start = ptr;
			while (*ptr && is_word_char(*ptr))
				ptr++;
			len = ptr - start;
			word = malloc(len + 1);
			if (!word)
				return (free_token_list(list), NULL);
			ft_strlcpy(word, start, len + 1);
			type.type = WORD;
			type.value = word;
			create_and_add(&list, type);
		}
		else
		{
			printf("minishell: syntax error near unexpected token `%c'\n",
				*ptr);
			free_token_list(list);
			return (NULL);
		}
	}
	return (list);
}
