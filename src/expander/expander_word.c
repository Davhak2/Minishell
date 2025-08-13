/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davihako <davihako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:19:45 by davihako          #+#    #+#             */
/*   Updated: 2025/08/13 11:19:46 by davihako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	handle_var_expansion(const char *word, size_t *i,
		t_expand *ctx)
{
	size_t	var_start;
	size_t	var_len;
	char	*var_name;

	var_start = *i;
	if (handle_var_number(word, i, ctx))
		return (1);
	while (word[*i] && (ft_isalnum(word[*i]) || word[*i] == '_'))
		(*i)++;
	var_len = *i - var_start;
	if (var_len > 0)
	{
		var_name = malloc(var_len + 1);
		if (!var_name)
			return (0);
		ft_memcpy(var_name, word + var_start, var_len);
		var_name[var_len] = '\0';
		copy_var_value(var_name, ctx);
		free(var_name);
	}
	else
		(*ctx->result)[(*ctx->j)++] = '$';
	return (1);
}

static int	process_word_char(const char *word, size_t *i, t_expand *ctx)
{
	if (word[*i] == '$')
	{
		(*i)++;
		if (!handle_var_expansion(word, i, ctx))
			return (0);
	}
	else
	{
		(*ctx->result)[(*ctx->j)] = word[(*i)++];
		(*ctx->j)++;
	}
	return (1);
}

char	*expand_word(const char *word, char **envp, int last_status)
{
	char			*result;
	size_t			i;
	size_t			j;
	t_expand		ctx;

	i = 0;
	result = init_result_buffer(word, envp, last_status);
	if (!result)
		return (NULL);
	init_expand_context(&ctx, &result, &j);
	set_context_env(&ctx, envp, last_status);
	while (word[i])
	{
		if (!process_word_char(word, &i, &ctx))
		{
			free(result);
			return (NULL);
		}
	}
	result[*ctx.j] = '\0';
	return (result);
}
