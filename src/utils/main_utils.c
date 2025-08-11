#include "minishell.h"

bool	has_unclosed_quote(const char *str)
{
	char	quote;

	quote = 0;
	while (*str)
	{
		if ((*str == '\'' || *str == '"') && !quote)
			quote = *str;
		else if (*str == quote)
			quote = 0;
		str++;
	}
	return (quote != 0);
}

char	*handle_multiline_input(char *input, t_shell *shell)
{
	char	*next;
	char	*tmp;
	char	*joined;

	while (has_unclosed_quote(input))
	{
		next = readline("> ");
		shell->heredoc_line++;
		if (!next)
		{
			free(input);
			return (NULL);
		}
		tmp = ft_strjoin(input, "\n");
		joined = ft_strjoin(tmp, next);
		free(tmp);
		free(next);
		free(input);
		input = joined;
	}
	return (input);
}

int	validate_and_parse(t_shell *shell, t_token **tokens, t_node **ast)
{
	t_token	*tokens_copy;

	if (validate_syntax(*tokens))
	{
		if (*tokens)
			free_token_list(*tokens);
		shell->token = NULL;
		shell->last_status = 2;
		return (1);
	}
	tokens_copy = *tokens;
	*ast = parse(&tokens_copy);
	shell->node = *ast;
	shell->redir = NULL;
	if (!*ast)
	{
		if (*tokens)
			free_token_list(*tokens);
		shell->token = NULL;
		return (1);
	}
	return (0);
}

void	cleanup_resources(t_node *ast, t_token *tokens, t_shell *shell)
{
	if (ast)
		free_ast(ast);
	if (tokens)
		free_token_list(tokens);
	shell->node = NULL;
	shell->token = NULL;
}

t_shell	*init_minishell(char **envp, char ***my_envp)
{
	t_shell	*shell;

	*my_envp = duplicate_envp(envp);
	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->envp = my_envp;
	shell->last_status = 0;
	shell->heredoc_line = 1;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	init_signals();
	return (shell);
}
