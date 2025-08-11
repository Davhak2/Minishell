/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:59:25 by letto             #+#    #+#             */
/*   Updated: 2025/08/11 21:17:39 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_slice(char **dest, const char *start, size_t len)
{
	char	*tmp;
	char	*joined;

	tmp = ft_substr(start, 0, len);
	if (!tmp)
		return (-1);
	joined = ft_strjoin(*dest, tmp);
	free(tmp);
	if (!joined)
	{
		free(*dest);
		*dest = NULL;
		return (-1);
	}
	free(*dest);
	*dest = joined;
	return (0);
}

int	read_quoted(char **p, t_tokens *qt, char **res)
{
	char	quote;
	char	*start;

	quote = **p;
	if (*qt == WORD && (quote == '\'' || quote == '"'))
	{
		if (quote == '\'')
			*qt = SINGLE_QUOTED;
		else if (quote == '"')
			*qt = DOUBLE_QUOTED;
	}
	(*p)++;
	start = *p;
	while (**p && **p != quote)
		(*p)++;
	if (!**p)
	{
		printf("minishell: syntax error: unclosed quote\n");
		free(*res);
		return (-1);
	}
	if (append_slice(res, start, (size_t)(*p - start)) < 0)
		return (-1);
	(*p)++;
	return (0);
}
