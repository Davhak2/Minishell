#include "expander.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>

static char	*expand_word(const char *word, char **envp, int last_status);

static char	*expand_var(const char *str, char **envp, int last_status)
{
	char	*buf;

	if (*str == '?' && *(str + 1) == '\0')
	{
		buf = ft_itoa(last_status);
		return (buf);
	}
	buf = get_env_value(str, envp);
	if (!buf)
		buf = ft_strdup("");
	return (buf);
}

static char	*expand_word(const char *word, char **envp, int last_status)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	len;
	size_t	var_start;
	char	*val;
	size_t	vlen;
	size_t	var_len;
	char	var_name[128];

	i = 0;
	j = 0;
	len = ft_strlen(word);
	result = malloc(len * 2 + 1);
	if (!result)
		return (NULL);
	while (word[i])
	{
		if (word[i] == '$')
		{
			i++;
			var_start = i;
			if (word[i] == '?')
			{
				val = expand_var("?", envp, last_status);
				vlen = ft_strlen(val);
				memcpy(result + j, val, vlen);
				j += vlen;
				free(val);
				i++;
			}
			else
			{
				while (word[i] && (ft_isalnum(word[i]) || word[i] == '_'))
					i++;
				var_len = i - var_start;
				ft_strlcpy(var_name, word + var_start, var_len + 1);
				val = expand_var(var_name, envp, last_status);
				vlen = ft_strlen(val);
				memcpy(result + j, val, vlen);
				j += vlen;
				free(val);
			}
		}
		else
		{
			result[j++] = word[i++];
		}
	}
	result[j] = '\0';
	return (result);
}

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