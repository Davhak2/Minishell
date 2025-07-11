#include "parser.h"

void	free_redirects(t_redirect *redir)
{
    t_redirect *tmp;
    while (redir)
    {
        tmp = redir->next;
        // TODO: free filename?
        free(redir);
        redir = tmp;
    }
}

void	free_ast(t_node *node)
{
    if (!node)
        return;
    free_ast(node->left);
    free_ast(node->right);
    if (node->type == WORD && node->value)
    {
        t_cmd *cmd = (t_cmd *)node->value;
        if (cmd->args)
            free(cmd->args);
        if (cmd->redirects)
            free_redirects(cmd->redirects);
        free(cmd);
    }
    free(node);
}