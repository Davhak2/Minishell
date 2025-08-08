#include "parser.h"
#include "utils.h"
#include "minishell.h"

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	// if (shell->cmd)
	// 	free_cmd(shell->cmd);
	if (shell->redir)
		free_redirects(shell->redir);
	if (shell->token)
		free_token_list(shell->token);
	if (shell->node)
		free_ast(shell->node);
	if (shell->envp && *(shell->envp))
		free_envp(*(shell->envp));
	// Close backup file descriptors if they exist
	if (shell->stdin_backup >= 0)
		close(shell->stdin_backup);
	if (shell->stdout_backup >= 0)
		close(shell->stdout_backup);
	free(shell);
}
