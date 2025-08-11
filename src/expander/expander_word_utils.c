#include "minishell.h"

void	set_context_pointers(t_expand *ctx, char **result, size_t *j)
{
	ctx->result = result;
	ctx->j = j;
}

void	set_context_env(t_expand *ctx, char **envp, int last_status)
{
	ctx->envp = envp;
	ctx->last_status = last_status;
}

void	copy_var_value(char *var_name, t_expand *ctx)
{
	char	*val;
	size_t	vlen;

	val = expand_var(var_name, ctx->envp, ctx->last_status);
	if (val)
	{
		vlen = ft_strlen(val);
		ft_memcpy(*ctx->result + *ctx->j, val, vlen);
		*ctx->j += vlen;
		free(val);
	}
}

size_t	handle_question_mark(char **envp, int last_status, size_t *i)
{
	char	*val;
	size_t	len;

	val = expand_var("?", envp, last_status);
	len = 0;
	if (val)
	{
		len = ft_strlen(val);
		free(val);
	}
	(*i)++;
	return (len);
}

size_t	calculate_expanded_size(const char *word, char **envp, int last_status)
{
	size_t		i;
	size_t		total_size;
	t_size		ctx;

	i = 0;
	total_size = 0;
	ctx.envp = envp;
	ctx.last_status = last_status;
	while (word[i])
	{
		if (word[i] == '$')
			process_dollar_expansion(word, &i, &total_size, &ctx);
		else
			process_regular_char(&i, &total_size);
	}
	return (total_size);
}
