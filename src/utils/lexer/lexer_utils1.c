/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:18:10 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 15:58:03 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include "utils.h"

void	create_and_add(t_token **list, t_type type)
{
	t_token	*token;

	token = create_token(type.type, type.value);
	if (!token)
	{
		free(type.value);
		return ;
	}
	add_token_to_list(list, token);
}

void	syntax_exit(char ptr, t_token *list)
{
	printf("minishell: syntax error near unexpected token `%c'\n", ptr);
	free_token_list(list);
}

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
