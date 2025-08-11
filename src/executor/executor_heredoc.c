#include "minishell.h"

static int	process_single_heredoc(t_redirect *current, t_shell *shell,
		int *heredoc_counter)
{
	int		result;
	char	*filename;

	filename = generate_heredoc_filename((*heredoc_counter)++);
	if (!filename)
		return (1);
	result = handle_heredoc_fork(current->filename, filename, shell, NULL);
	if (result != 0)
	{
		unlink(filename);
		free(filename);
		return (result);
	}
	free(current->heredoc_filename);
	current->heredoc_filename = filename;
	return (0);
}

static int	process_heredocs_in_cmd(t_cmd *cmd, t_shell *shell,
		int *heredoc_counter)
{
	t_redirect	*current;
	int			result;

	if (!cmd || !cmd->redirects)
		return (0);
	current = cmd->redirects;
	while (current)
	{
		if (current->type == REDIRECT_HEREDOC)
		{
			result = process_single_heredoc(current, shell, heredoc_counter);
			if (result != 0)
				return (result);
		}
		current = current->next;
	}
	return (0);
}

static int	process_binary_node_heredocs(t_node *node, t_shell *shell,
		int *heredoc_counter)
{
	int	result;

	result = process_all_heredocs(node->left, shell, heredoc_counter);
	if (result != 0)
		return (result);
	return (process_all_heredocs(node->right, shell, heredoc_counter));
}

int	process_all_heredocs(t_node *node, t_shell *shell, int *heredoc_counter)
{
	t_cmd	*cmd;

	if (!node)
		return (0);
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		return (process_heredocs_in_cmd(cmd, shell, heredoc_counter));
	}
	else if (node->type == PIPE || node->type == AND || node->type == OR)
		return (process_binary_node_heredocs(node, shell, heredoc_counter));
	return (0);
}
