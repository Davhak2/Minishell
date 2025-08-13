/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_word_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davihako <davihako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:19:39 by davihako          #+#    #+#             */
/*   Updated: 2025/08/13 11:19:40 by davihako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*init_result_buffer(const char *word, char **envp, int last_status)
{
	size_t		result_size;
	char		*result;

	result_size = calculate_expanded_size(word, envp, last_status);
	result = malloc(result_size + 1);
	return (result);
}

void	init_expand_context(t_expand *ctx, char **result, size_t *j)
{
	*j = 0;
	set_context_pointers(ctx, result, j);
}

int	handle_var_number(const char *word, size_t *i, t_expand *ctx)
{
	char	*val;
	size_t	vlen;

	if (word[*i] == '?')
	{
		val = expand_var("?", ctx->envp, ctx->last_status);
		if (val)
		{
			vlen = ft_strlen(val);
			ft_memcpy(*ctx->result + *ctx->j, val, vlen);
			*ctx->j += vlen;
			free(val);
		}
		(*i)++;
		return (1);
	}
	return (0);
}
