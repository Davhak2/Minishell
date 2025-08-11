#include "minishell.h"

void	iterate(int *i1, int *i2)
{
	(*i1)++;
	(*i2)++;
}

static int	count_wildcard_matches(char *arg)
{
	char	**wildcards;
	int		count;
	int		j;

	count = 0;
	wildcards = wildcard_expand(arg);
	if (wildcards)
	{
		j = 0;
		while (wildcards[j])
			iterate(&count, &j);
		ft_free_array(wildcards);
	}
	else
		count = 1;
	return (count);
}

int	count_wildcards(t_cmd *cmd)
{
	int		new_argc;
	int		i;

	new_argc = 0;
	i = -1;
	while (cmd->args[++i])
	{
		if (cmd->arg_types[i] == WORD && ft_strchr(cmd->args[i], '*'))
			new_argc += count_wildcard_matches(cmd->args[i]);
		else
			new_argc++;
	}
	return (new_argc);
}

void	copy_wildcard_matches(char **wildcards, char **new_args,
		t_tokens *new_types, int *k)
{
	int	j;

	j = 0;
	while (wildcards[j])
	{
		new_args[*k] = ft_strdup(wildcards[j]);
		new_types[*k] = WORD;
		(*k)++;
		j++;
	}
	ft_free_array(wildcards);
}

void	copy_regular_arg(t_cmd *cmd, char **new_args,
		t_tokens *new_types, int *indices)
{
	new_args[indices[1]] = cmd->args[indices[0]];
	new_types[indices[1]] = cmd->arg_types[indices[0]];
	indices[1]++;
}
