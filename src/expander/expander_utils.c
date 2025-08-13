/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davihako <davihako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:19:37 by davihako          #+#    #+#             */
/*   Updated: 2025/08/13 11:19:38 by davihako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var(const char *str, char **envp, int last_status)
{
	char	*buf;

	if (*str == '?' && *(str + 1) == '\0')
	{
		buf = ft_itoa(last_status);
		return (buf);
	}
	buf = get_env_value(str, envp);
	if (!buf)
		buf = ft_strdup("");
	return (buf);
}

char	*get_env_value(const char *name, char **envp)
{
	size_t	name_len;
	int		i;

	name_len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, name_len)
			&& envp[i][name_len] == '=')
			return (ft_strdup(envp[i] + name_len + 1));
		++i;
	}
	return (NULL);
}

static size_t	extract_var_name(const char *word, size_t *i, char **var_name)
{
	size_t	var_start;
	size_t	var_len;

	var_start = *i;
	while (word[*i] && (ft_isalnum(word[*i]) || word[*i] == '_'))
		(*i)++;
	var_len = *i - var_start;
	if (var_len > 0)
	{
		*var_name = malloc(var_len + 1);
		if (*var_name)
		{
			ft_memcpy(*var_name, word + var_start, var_len);
			(*var_name)[var_len] = '\0';
		}
	}
	return (var_len);
}

static size_t	handle_variable_name(const char *word, size_t *i,
		char **envp, int last_status)
{
	char	*var_name;
	char	*val;
	size_t	len;
	size_t	var_len;

	var_name = NULL;
	var_len = extract_var_name(word, i, &var_name);
	if (var_len > 0 && var_name)
	{
		val = expand_var(var_name, envp, last_status);
		len = 0;
		if (val)
		{
			len = ft_strlen(val);
			free(val);
		}
		free(var_name);
		return (len);
	}
	return (1);
}

size_t	get_var_value_len(const char *word, size_t *i, char **envp,
		int last_status)
{
	if (word[*i] == '?')
		return (handle_question_mark(envp, last_status, i));
	return (handle_variable_name(word, i, envp, last_status));
}
