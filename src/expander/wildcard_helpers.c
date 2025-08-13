/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davihako <davihako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:19:59 by davihako          #+#    #+#             */
/*   Updated: 2025/08/13 11:20:00 by davihako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**resize_result_array(char **result, int count, int *cap)
{
	char	**tmp;
	int		i;

	*cap *= 2;
	tmp = malloc(sizeof(char *) * (*cap));
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		tmp[i] = result[i];
		i++;
	}
	free(result);
	return (tmp);
}

int	should_skip_entry(struct dirent *entry, const char *filename_pattern)
{
	if (entry->d_name[0] == '.' && filename_pattern[0] != '.')
		return (1);
	if (!wildcard_match(filename_pattern, entry->d_name))
		return (1);
	if (!ft_strcmp(entry->d_name, ".") || !ft_strcmp(entry->d_name, ".."))
		return (1);
	return (0);
}

char	*create_full_path(const char *dir_path, const char *filename)
{
	char	*full_path;
	char	*result;

	if (!ft_strcmp(dir_path, "."))
		return (ft_strdup(filename));
	full_path = ft_strjoin(dir_path, "/");
	if (!full_path)
		return (NULL);
	result = ft_strjoin(full_path, filename);
	free(full_path);
	return (result);
}

void	process_dollar_expansion(const char *word, size_t *i,
		size_t *total_size, t_size *ctx)
{
	(*i)++;
	*total_size += get_var_value_len(word, i, ctx->envp, ctx->last_status);
}

void	process_regular_char(size_t *i, size_t *total_size)
{
	(*total_size)++;
	(*i)++;
}
