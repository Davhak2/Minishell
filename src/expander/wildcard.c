/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davihako <davihako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:20:08 by davihako          #+#    #+#             */
/*   Updated: 2025/08/13 11:20:09 by davihako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_entry(t_process *ctx, struct dirent *entry)
{
	if (*ctx->count + 2 > *ctx->cap)
	{
		*ctx->result = resize_result_array(*ctx->result, *ctx->count,
				ctx->cap);
		if (!*ctx->result)
			return (0);
	}
	(*ctx->result)[*ctx->count] = create_full_path(ctx->dir_path,
			entry->d_name);
	if (!(*ctx->result)[*ctx->count])
		return (0);
	(*ctx->count)++;
	return (1);
}

static char	**init_result_array(void)
{
	return (malloc(sizeof(char *) * 8));
}

static char	**finalize_result(char **result, int count)
{
	if (count == 0)
		return (free(result), NULL);
	result[count] = NULL;
	return (result);
}

static char	**process_directory(DIR *dir, const char *dir_path,
		const char *filename_pattern)
{
	struct dirent	*entry;
	char			**result;
	int				count;
	int				cap;
	t_process		ctx;

	result = init_result_array();
	if (!result)
		return (NULL);
	count = 0;
	cap = 8;
	ctx = (t_process){&result, &count, &cap, dir_path};
	entry = readdir(dir);
	while (entry)
	{
		if (should_skip_entry(entry, filename_pattern))
		{
			entry = readdir(dir);
			continue ;
		}
		if (!process_entry(&ctx, entry))
			return (ft_free_array(result), NULL);
		entry = readdir(dir);
	}
	return (finalize_result(result, count));
}

char	**wildcard_expand(const char *pattern)
{
	DIR		*dir;
	char	**result;
	char	*dir_path;
	char	*filename_pattern;

	dir_path = get_dir_from_pattern(pattern);
	filename_pattern = get_filename_pattern(pattern);
	if (!dir_path || !filename_pattern)
		return (free(dir_path), free(filename_pattern), NULL);
	dir = opendir(dir_path);
	if (!dir)
		return (free(dir_path), free(filename_pattern), NULL);
	result = process_directory(dir, dir_path, filename_pattern);
	closedir(dir);
	free(dir_path);
	free(filename_pattern);
	return (result);
}
