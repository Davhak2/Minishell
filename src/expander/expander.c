#include "expander.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>

static char	*expand_word(const char *word, char **envp, int last_status);
static char	*get_env_value(const char *name, char **envp)
{
	size_t	name_len;
	int		i;

	name_len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, name_len) && envp[i][name_len] == '=')
			return (ft_strdup(envp[i] + name_len + 1));
		++i;
	}
	return (NULL);
}
static char	*expand_var(const char *str, char **envp, int last_status);
static bool	is_single_quote(const char *str);

void	expand_ast(t_node *node, char **envp, int last_status)
{
	if (!node)
		return ;
	if (node->type == WORD && node->value)
	{
		t_cmd *cmd = (t_cmd *)node->value;
		int i = 0;
		while (cmd->args && cmd->args[i])
		{
			char *expanded = expand_word(cmd->args[i], envp, last_status);
			free(cmd->args[i]);
			cmd->args[i] = expanded;
			i++;
		}
		t_redirect *redir = cmd->redirects;
		while (redir)
		{
			char *expanded = expand_word(redir->filename, envp, last_status);
			free(redir->filename);
			redir->filename = expanded;
			redir = redir->next;
		}
	}
	expand_ast(node->left, envp, last_status);
	expand_ast(node->right, envp, last_status);
}