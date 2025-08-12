#include "minishell.h"

t_segment	*create_segment(t_tokens type, char *value)
{
	t_segment	*segment;

	segment = malloc(sizeof(t_segment));
	if (!segment)
		return (NULL);
	segment->type = type;
	segment->value = ft_strdup(value);
	segment->next = NULL;
	if (!segment->value)
	{
		free(segment);
		return (NULL);
	}
	return (segment);
}

void	add_segment(t_segment **head, t_segment *new_segment)
{
	t_segment	*current;

	if (!*head)
	{
		*head = new_segment;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_segment;
}

static int	parse_quoted_content_ctx(char **ptr, char **result,
		char **temp_value)
{
	char	quote_char;
	char	*start;

	quote_char = **ptr;
	(*ptr)++;
	start = *ptr;
	while (**ptr && **ptr != quote_char)
		(*ptr)++;
	if (!**ptr)
		return (printf("minishell: syntax error: unclosed quote\n"),
			free(*result), -1);
	*temp_value = ft_substr(start, 0, *ptr - start);
	if (!*temp_value)
		return (free(*result), -1);
	(*ptr)++;
	return (quote_char);
}

static int	add_quoted_segment_ctx(t_segment **segments, char **result,
		char *temp_value, int quote_char)
{
	t_segment	*segment;
	int			type;

	if (quote_char == '\'')
		type = SINGLE_QUOTED;
	else
		type = DOUBLE_QUOTED;
	segment = create_segment(type, temp_value);
	if (!segment)
		return (free(*result), free(temp_value), -1);
	add_segment(segments, segment);
	if (append_slice(result, temp_value, ft_strlen(temp_value)) < 0)
		return (free(temp_value), -1);
	free(temp_value);
	return (0);
}

int	handle_quoted(char **ptr, t_segment **segments,
		char **result, int *has_segments)
{
	int		quote_char;
	char	*temp_value;
	int		ret;

	quote_char = parse_quoted_content_ctx(ptr, result, &temp_value);
	if (quote_char < 0)
		return (-1);
	ret = add_quoted_segment_ctx(segments, result, temp_value, quote_char);
	if (ret == 0)
		*has_segments = 1;
	return (ret);
}
