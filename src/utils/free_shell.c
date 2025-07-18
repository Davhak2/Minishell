#include "minishell.h"
#include "utils.h"
#include "parser.h"

void free_shell(t_shell *shell)
{
	if (!shell)
		return;
	// if (shell->cmd)
	// 	free_cmd(shell->cmd);
	if (shell->node)
		free_ast(shell->node);
	if (shell->redir)
		free_redirects(shell->redir);
	if (shell->token)
		free_token_list(shell->token);
	free(shell);
}
