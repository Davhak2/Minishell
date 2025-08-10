#include "expander.h"
#include "libft.h"
#include "parser.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static char		*expand_word(const char *word, char **envp, int last_status);

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

char	*expand_tilde(char *arg, char **envp)
{
	char	*home;
	char	*expanded;
	char	*user;
	char	*default_home;
	char	*pwd_value;

	if (!arg || *arg != '~')
		return (ft_strdup(arg));
	if (*(arg + 1) == '+' && (*(arg + 2) == '\0' || *(arg + 2) == '/'))
	{
		pwd_value = get_env_value("PWD", envp);
		if (!pwd_value)
			return (ft_strdup(arg));
		if (*(arg + 2) == '\0')
			expanded = ft_strdup(pwd_value);
		else
			expanded = ft_strjoin(pwd_value, arg + 2);
		free(pwd_value);
		return (expanded);
	}
	if (*(arg + 1) == '-' && (*(arg + 2) == '\0' || *(arg + 2) == '/'))
	{
		pwd_value = get_env_value("OLDPWD", envp);
		if (!pwd_value)
			return (ft_strdup(arg));
		if (*(arg + 2) == '\0')
			expanded = ft_strdup(pwd_value);
		else
			expanded = ft_strjoin(pwd_value, arg + 2);
		free(pwd_value);
		return (expanded);
	}
	if (*(arg + 1) == '\0' || *(arg + 1) == '/')
	{
		home = get_env_value("HOME", envp);
		if (!home)
		{
			user = get_env_value("USER", envp);
			if (user)
			{
				default_home = ft_strjoin("/home/", user);
				free(user);
				if (default_home)
					home = default_home;
			}
		}
		if (!home)
			return (ft_strdup(arg));
		if (*(arg + 1) == '\0')
			expanded = ft_strdup(home);
		else
			expanded = ft_strjoin(home, arg + 1);
		free(home);
		return (expanded);
	}
	return (ft_strdup(arg));
}

static size_t	calculate_expanded_size(const char *word, char **envp,
		int last_status)
{
	size_t	i;
	size_t	total_size;
	size_t	var_start;
	char	*val;
	size_t	var_len;
	char	*var_name;

	i = 0;
	total_size = 0;
	while (word[i])
	{
		if (word[i] == '$')
		{
			i++;
			var_start = i;
			if (word[i] == '?')
			{
				val = expand_var("?", envp, last_status);
				if (val)
				{
					total_size += ft_strlen(val);
					free(val);
				}
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
					if (var_name)
					{
						ft_memcpy(var_name, word + var_start, var_len);
						var_name[var_len] = '\0';
						val = expand_var(var_name, envp, last_status);
						if (val)
						{
							total_size += ft_strlen(val);
							free(val);
						}
						free(var_name);
					}
				}
				else
				{
					total_size += 1;
				}
			}
		}
		else
		{
			total_size += 1;
			i++;
		}
	}
	return (total_size);
}

static char	*expand_word(const char *word, char **envp, int last_status)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	var_start;
	char	*val;
	size_t	vlen;
	size_t	var_len;
	char	*var_name;
	size_t	result_size;

	i = 0;
	j = 0;
	result_size = calculate_expanded_size(word, envp, last_status);
	result = malloc(result_size + 1);
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
				if (val)
				{
					vlen = ft_strlen(val);
					ft_memcpy(result + j, val, vlen);
					j += vlen;
					free(val);
				}
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
					if (val)
					{
						vlen = ft_strlen(val);
						ft_memcpy(result + j, val, vlen);
						j += vlen;
						free(val);
					}
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

static int	wildcard_match(const char *pattern, const char *str)
{
	while (*pattern && *str)
	{
		if (*pattern == '*')
		{
			pattern++;
			if (!*pattern)
				return (1);
			while (*str)
			{
				if (wildcard_match(pattern, str))
					return (1);
				str++;
			}
			return (0);
		}
		else if (*pattern == *str)
		{
			pattern++;
			str++;
		}
		else
			return (0);
	}
	if (*pattern == '*')
		++pattern;
	return (!*pattern && !*str);
}

static char	**wildcard_expand(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**result;
	int				count;
	int				cap;
	int				i;
	char			**tmp;

	result = NULL;
	count = 0;
	cap = 8;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	result = malloc(sizeof(char *) * cap);
	if (!result)
	{
		closedir(dir);
		return (NULL);
	}
	while ((entry = readdir(dir)))
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (wildcard_match(pattern, entry->d_name))
		{
			if (!ft_strcmp(entry->d_name, ".") || !ft_strcmp(entry->d_name, ".."))
				continue ;
			if (count + 2 > cap)
			{
				cap *= 2;
				tmp = malloc(sizeof(char *) * cap);
				if (!tmp)
				{
					ft_free_array(result);
					closedir(dir);
					return (NULL);
				}
				for (i = 0; i < count; i++)
					tmp[i] = result[i];
				free(result);
				result = tmp;
			}
			result[count++] = ft_strdup(entry->d_name);
		}
	}
	closedir(dir);
	if (count == 0)
		return (free(result), NULL);
	result[count] = NULL;
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

void	expand_ast(t_node *node, char **envp, t_shell *shell)
{
	t_cmd		*cmd;
	char		*expanded;
	t_redirect	*redir;
	char		**wildcards;
	int			new_argc;
	char		**new_args;
	t_tokens	*new_types;

	int i, j, k;
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
					expanded = expand_word(cmd->args[i], envp,
							shell->last_status);
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
				i++;
			}
			new_argc = 0;
			for (i = 0; cmd->args[i]; ++i)
			{
				if (cmd->arg_types[i] == WORD && ft_strchr(cmd->args[i], '*'))
				{
					wildcards = wildcard_expand(cmd->args[i]);
					if (wildcards)
					{
						j = -1;
						while (wildcards[++j])
							new_argc++;
						j = -1;
						ft_free_array(wildcards);
					}
					else
						new_argc++;
				}
				else
					new_argc++;
			}
			new_args = malloc(sizeof(char *) * (new_argc + 1));
			new_types = malloc(sizeof(t_tokens) * (new_argc + 1));
			k = 0;
			for (i = 0; cmd->args[i]; ++i)
			{
				if (cmd->arg_types[i] == WORD && ft_strchr(cmd->args[i], '*'))
				{
					wildcards = wildcard_expand(cmd->args[i]);
					if (wildcards)
					{
						for (j = 0; wildcards[j]; ++j)
						{
							new_args[k] = ft_strdup(wildcards[j]);
							new_types[k] = WORD;
							k++;
						}
						ft_free_array(wildcards);
						free(cmd->args[i]);
					}
					else
					{
						new_args[k] = cmd->args[i];
						new_types[k] = cmd->arg_types[i];
						k++;
					}
				}
				else
				{
					new_args[k] = cmd->args[i];
					new_types[k] = cmd->arg_types[i];
					k++;
				}
			}
			new_args[k] = NULL;
			free(cmd->args);
			free(cmd->arg_types);
			cmd->args = new_args;
			cmd->arg_types = new_types;
		}
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->filename)
			{
				expanded = expand_word(redir->filename, envp,
						shell->last_status);
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
			redir = redir->next;
		}
	}
	if (node->left)
		expand_ast(node->left, envp, shell);
	if (node->right)
		expand_ast(node->right, envp, shell);
}
