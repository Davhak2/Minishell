#include "executor.h"
#include "libft.h"
#include "parser.h"
#include "signals.h"
#include "utils.h"

char	*get_token_type_name(t_tokens type)
{
	static char *token_names[] = {
		"WORD",             // 0
		"REDIRECT_IN",      // 1
		"REDIRECT_OUT",     // 2
		"REDIRECT_APPEND",  // 3
		"REDIRECT_HEREDOC", // 4
		"PIPE",             // 5
		"AND",              // 6
		"OR",               // 7
		"LPAREN",           // 8
		"RPAREN",           // 9
		"EOF_",             // 10
		"SINGLE_QUOTE",     // 11
	};
	if (type >= 0 && type <= sizeof(token_names) / sizeof(token_names[0]))
	{
		return (token_names[type]);
	}
	return ("UNKNOWN");
}

void	print_redirects(t_redirect *redir, int depth)
{
	char	*redir_type;

	while (redir)
	{
		redir_type = get_token_type_name(redir->type);
		for (int i = 0; i < depth + 1; i++)
			printf("  ");
		printf("↳ redirect: %s %s\n", redir_type,
			redir->filename ? redir->filename : "(null)");
		redir = redir->next;
	}
}
void	print_ast(t_node *node, int depth)
{
	const char	*type;
	t_cmd		*cmd;

	if (!node)
		return ;
	type = get_token_type_name(node->type);
	for (int i = 0; i < depth; i++)
		printf("  ");
	printf("└─ %s", type);
	if (node->type == WORD && node->value)
	{
		cmd = (t_cmd *)node->value;
		printf(": ");
		if (cmd->args)
			for (int i = 0; cmd->args[i]; i++)
				printf("%s ", cmd->args[i]);
		printf("\n");
		print_redirects(cmd->redirects, depth);
	}
	else
	{
		printf("\n");
	}
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}

void	print_tokens(t_token *tokens)
{
	int	token_count;

	token_count = 0;
	printf("\n🔍 \033[1;36mTokens found:\033[0m\n");
	printf("┌─────┬─────────────────┬─────────────────┐\n");
	printf("│   № │ Type            │ Value           │\n");
	printf("├─────┼─────────────────┼─────────────────┤\n");
	while (tokens)
	{
		printf("│ %-3d │ %-15s │ %-15s │\n", ++token_count,
			get_token_type_name(tokens->type),
			tokens->value ? tokens->value : "(null)");
		tokens = tokens->next;
	}
	printf("└─────┴─────────────────┴─────────────────┘\n");
	printf("Total tokens: %d\n\n", token_count);
}

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

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	t_token	*tokens_copy;
	t_node	*ast;
	char	*next;
	char	*tmp;
	char	*joined;
	char	*test;
	t_shell	*shell;
	char	**my_envp;

	(void)argc;
	(void)argv;
	my_envp = duplicate_envp(envp);
	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (1);
	shell->envp = &my_envp;
	shell->last_status = 0;
	shell->heredoc_line = 1;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	init_signals();
	while (1)
	{
		g_received_signal = 0;
		input = readline("\001\033[1;32m\002🐚 ms: ➜\001\033[0m\002 ");
		shell->heredoc_line++;
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		while (has_unclosed_quote(input))
		{
			next = readline("> ");
			shell->heredoc_line++;
			if (!next)
			{
				free(input);
				break ;
			}
			tmp = ft_strjoin(input, "\n");
			joined = ft_strjoin(tmp, next);
			free(tmp);
			free(next);
			free(input);
			input = joined;
		}
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			tokens = tokenize(input);
			shell->token = tokens;
			// print_tokens(tokens);
			if (validate_syntax(tokens))
			{
				if (tokens)
					free_token_list(tokens);
				shell->token = NULL;
				shell->last_status = 2;
				free(input);
				continue ;
			}
			tokens_copy = tokens;
			ast = parse(&tokens_copy);
			shell->node = ast;
			shell->redir = NULL;
			if (!ast)
			{
				if (tokens)
					free_token_list(tokens);
				shell->token = NULL;
				free(input);
				continue ;
			}
			// printf("\n🌳 \033[1;35mAST:\033[0m\n");
			// print_ast(ast, 0);
			execute_ast(ast, shell);
			// printf("\n\n🌳 \033[1;35mAST after expand:\033[0m\n");
			// print_ast(ast, 0);
			// Clean up after each command
			if (ast)
				free_ast(ast);
			if (tokens)
				free_token_list(tokens);
			// Reset shell pointers to avoid double free
			shell->node = NULL;
			shell->token = NULL;
		}
		free(input);
	}
	if (my_envp)
		free_envp(my_envp);
	if (shell)
		free(shell);
	return (0);
}
