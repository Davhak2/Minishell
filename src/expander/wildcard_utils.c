/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davihako <davihako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:20:02 by davihako          #+#    #+#             */
/*   Updated: 2025/08/13 11:20:03 by davihako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_wildcard_pattern(const char **pattern, const char **str)
{
	(*pattern)++;
	if (!**pattern)
		return (1);
	while (**str)
	{
		if (wildcard_match(*pattern, *str))
			return (1);
		(*str)++;
	}
	return (0);
}

static int	handle_char_match(const char **pattern, const char **str)
{
	if (**pattern == **str)
	{
		(*pattern)++;
		(*str)++;
		return (1);
	}
	return (0);
}

int	wildcard_match(const char *pattern, const char *str)
{
	while (*pattern && *str)
	{
		if (*pattern == '*')
		{
			if (handle_wildcard_pattern(&pattern, &str))
				return (1);
			return (0);
		}
		else if (!handle_char_match(&pattern, &str))
			return (0);
	}
	if (*pattern == '*')
		++pattern;
	return (!*pattern && !*str);
}

char	*get_dir_from_pattern(const char *pattern)
{
	const char	*last_slash;
	char		*dir_path;

	last_slash = ft_strrchr(pattern, '/');
	if (!last_slash)
		return (ft_strdup("."));
	if (last_slash == pattern)
		return (ft_strdup("/"));
	dir_path = malloc(last_slash - pattern + 1);
	if (!dir_path)
		return (NULL);
	ft_memcpy(dir_path, pattern, last_slash - pattern);
	dir_path[last_slash - pattern] = '\0';
	return (dir_path);
}

char	*get_filename_pattern(const char *pattern)
{
	const char	*last_slash;

	last_slash = ft_strrchr(pattern, '/');
	if (!last_slash)
		return (ft_strdup(pattern));
	return (ft_strdup(last_slash + 1));
}
