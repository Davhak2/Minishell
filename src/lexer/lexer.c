/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:44:13 by luminous          #+#    #+#             */
/*   Updated: 2025/07/20 21:57:42 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"

t_token	*tokenize(char *line)
{
	t_token	*list;
	char	*ptr;
	t_type	type;
	char	*start;
	int		len;
	char	*word;
	char	quote;

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
			quote = *ptr;
			ptr++;
			start = ptr;
			while (*ptr && *ptr != quote)
				ptr++;
			if (!*ptr) // Unclosed quote
			{
				printf("minishell: syntax error: unclosed quote\n");
				free_token_list(list);
				return (NULL);
			}
			len = ptr - start;
			word = malloc(len + 1);
			if (!word)
				return (free_token_list(list), NULL);
			ft_strlcpy(word, start, len + 1);
			type.type = (quote == '\'') ? SINGLE_QUOTED : WORD;
			type.value = word;
			create_and_add(&list, type);
			ptr++; // Skip closing quote
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
