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
	char	*var_name;

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
				ft_memcpy(result + j, val, vlen);
				j += vlen;
				free(val);
				i++;
			}
			else
			{
				while (word[i] && (ft_isalnum(word[i]) || word[i] == '_'))
					i++;
				var_len = i - var_start;
				if (var_len > 0)
				{
					var_name = malloc(var_len + 1);
					if (!var_name)
					{
						free(result);
						return (NULL);
					}
					ft_memcpy(var_name, word + var_start, var_len);
					var_name[var_len] = '\0';
					val = expand_var(var_name, envp, last_status);
					vlen = ft_strlen(val);
					ft_memcpy(result + j, val, vlen);
					j += vlen;
					free(val);
					free(var_name);
				}
				else
				{
					result[j++] = '$';
				}
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

char	*get_env_value(const char *name, char **envp)
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
	t_cmd		*cmd;
	int			i;
	char		*expanded;
	t_redirect	*redir;

	if (!node)
		return ;
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		i = 0;
		if (cmd->args && cmd->arg_types)
		{
			while (cmd->args[i])
			{
				if (cmd->arg_types[i] != SINGLE_QUOTED)
				{
					expanded = expand_word(cmd->args[i], envp, last_status);
					if (expanded)
					{
						free(cmd->args[i]);
						cmd->args[i] = expanded;
					}
				}
				i++;
			}
		}
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->filename)
			{
				expanded = expand_word(redir->filename, envp, last_status);
				if (expanded)
				{
					free(redir->filename);
					redir->filename = expanded;
				}
			}
			redir = redir->next;
		}
	}
	if (node->left)
		expand_ast(node->left, envp, last_status);
	if (node->right)
		expand_ast(node->right, envp, last_status);
}
