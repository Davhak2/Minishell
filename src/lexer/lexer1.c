#include "minishell.h"

static t_segment	*create_segment(t_tokens type, char *value)
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

static void	add_segment(t_segment **head, t_segment *new_segment)
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

char	*process_quotes(char **ptr, t_tokens *quote_type, t_segment **segments)
{
	char		*result;
	t_segment	*segment;
	char		*temp_value;
	char		quote_char;
	char		*start;
	int			has_segments;

	result = ft_strdup("");
	has_segments = 0;
	if (!result)
		return (NULL);
	*quote_type = WORD;
	*segments = NULL;
	while (**ptr && !is_whitespace(**ptr) && !is_operator_char(**ptr))
	{
		if (is_quote(**ptr))
		{
			quote_char = **ptr;
			(*ptr)++;
			start = *ptr;
			while (**ptr && **ptr != quote_char)
				(*ptr)++;
			if (!**ptr)
			{
				printf("minishell: syntax error: unclosed quote\n");
				free(result);
				return (NULL);
			}
			temp_value = ft_substr(start, 0, *ptr - start);
			if (!temp_value)
				return (free(result), NULL);
			if (quote_char == '\'')
				segment = create_segment(SINGLE_QUOTED, temp_value);
			else
				segment = create_segment(DOUBLE_QUOTED, temp_value);
			if (!segment)
				return (free(result), free(temp_value), NULL);
			add_segment(segments, segment);
			has_segments = 1;
			if (append_slice(&result, temp_value, ft_strlen(temp_value)) < 0)
				return (free(temp_value), NULL);
			free(temp_value);
			(*ptr)++;
		}
		else
		{
			start = *ptr;
			while (**ptr && !is_whitespace(**ptr) && !is_quote(**ptr)
				&& !is_operator_char(**ptr))
				(*ptr)++;
			if (*ptr > start)
			{
				temp_value = ft_substr(start, 0, *ptr - start);
				if (!temp_value)
					return (free(result), NULL);
				segment = create_segment(WORD, temp_value);
				if (!segment)
					return (free(result), free(temp_value), NULL);
				add_segment(segments, segment);
				has_segments = 1;
				if (append_slice(&result, temp_value,
						ft_strlen(temp_value)) < 0)
					return (free(temp_value), NULL);
				free(temp_value);
			}
		}
	}
	if (has_segments)
	{
		free(result);
		return (ft_strdup(""));
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
