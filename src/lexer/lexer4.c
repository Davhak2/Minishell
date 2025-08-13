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

int	parse_quoted_content(char **ptr, char **result, char **temp_value)
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

int	add_quoted_segment(t_quote_data d)
{
	t_segment	*segment;
	int			type;

	if (d.quote_char == '\'')
		type = SINGLE_QUOTED;
	else
		type = DOUBLE_QUOTED;
	segment = create_segment(type, d.temp_value);
	if (!segment)
		return (free(*d.result), free(d.temp_value), -1);
	add_segment(d.segments, segment);
	*d.has_segments = 1;
	if (append_slice(d.result, d.temp_value, ft_strlen(d.temp_value)) < 0)
		return (free(d.temp_value), -1);
	free(d.temp_value);
	return (0);
}

int	handle_quoted(char **ptr, t_segment **segments,
		char **result, int *has_segments)
{
	char			quote_char;
	char			*temp_value;
	t_quote_data	data;

	quote_char = parse_quoted_content(ptr, result, &temp_value);
	if (quote_char < 0)
		return (-1);
	data.segments = segments;
	data.result = result;
	data.temp_value = temp_value;
	data.quote_char = quote_char;
	data.has_segments = has_segments;
	return (add_quoted_segment(data));
}
