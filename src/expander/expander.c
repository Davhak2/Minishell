#include "minishell.h"

static void	expand_single_arg(t_cmd *cmd, int i, char **envp, t_shell *shell)
{
	char	*expanded;

	expanded = expand_word(cmd->args[i], envp, shell->last_status);
	if (expanded)
	{
		free(cmd->args[i]);
		cmd->args[i] = expanded;
	}
	if (cmd->args[i] && cmd->args[i][0] == '~')
	{
		expanded = expand_tilde(cmd->args[i], envp);
		if (expanded)
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
	}
}

static void	expand_redirect_filename(t_redirect *redir, char **envp,
		t_shell *shell)
{
	char	*expanded;

	expanded = expand_word(redir->filename, envp, shell->last_status);
	if (expanded && expanded != redir->filename)
	{
		free(redir->filename);
		redir->filename = expanded;
	}
	if (redir->filename && redir->filename[0] == '~')
	{
		expanded = expand_tilde(redir->filename, envp);
		if (expanded && expanded != redir->filename)
		{
			free(redir->filename);
			redir->filename = expanded;
		}
	}
}

void	expand_args(t_cmd *cmd, char **envp, t_shell *shell)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (cmd->arg_types[i] != SINGLE_QUOTED)
			expand_single_arg(cmd, i, envp, shell);
		i++;
	}
}

void	expand_redirects(t_cmd *cmd, char **envp, t_shell *shell)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->filename)
			expand_redirect_filename(redir, envp, shell);
		redir = redir->next;
	}
}

void	expand_ast(t_node *node, char **envp, t_shell *shell)
{
	t_cmd	*cmd;

	if (!node)
		return ;
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		if (cmd->args && cmd->arg_types)
		{
			expand_args(cmd, envp, shell);
			handle_wildcard_expansion(cmd);
		}
		expand_redirects(cmd, envp, shell);
	}
	if (node->left)
		expand_ast(node->left, envp, shell);
	if (node->right)
		expand_ast(node->right, envp, shell);
}
