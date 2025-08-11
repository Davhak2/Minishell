#include "minishell.h"

static void	fill_wildcard_args(t_cmd *cmd, char **new_args, t_tokens *new_types)
{
	char	**wildcards;
	int		indices[2];

	indices[0] = 0;
	indices[1] = 0;
	while (cmd->args[indices[0]])
	{
		if (cmd->arg_types[indices[0]] == WORD
			&& ft_strchr(cmd->args[indices[0]], '*'))
		{
			wildcards = wildcard_expand(cmd->args[indices[0]]);
			if (wildcards)
			{
				copy_wildcard_matches(wildcards, new_args, new_types,
					&indices[1]);
				free(cmd->args[indices[0]]);
			}
			else
				copy_regular_arg(cmd, new_args, new_types, indices);
		}
		else
			copy_regular_arg(cmd, new_args, new_types, indices);
		indices[0]++;
	}
	new_args[indices[1]] = NULL;
}

void	handle_wildcard_expansion(t_cmd *cmd)
{
	int			new_argc;
	char		**new_args;
	t_tokens	*new_types;

	new_argc = count_wildcards(cmd);
	new_args = malloc(sizeof(char *) * (new_argc + 1));
	new_types = malloc(sizeof(t_tokens) * (new_argc + 1));
	if (!new_args || !new_types)
		return ;
	fill_wildcard_args(cmd, new_args, new_types);
	free(cmd->args);
	free(cmd->arg_types);
	cmd->args = new_args;
	cmd->arg_types = new_types;
}
