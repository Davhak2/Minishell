#include "minishell.h"
#include "parser.h"
#include "utils.h"

char *get_token_type_name(t_tokens type)
{
	static char *token_names[] = {
		"WORD",				// 0
		"REDIRECT_IN",		// 1
		"REDIRECT_OUT",		// 2
		"REDIRECT_APPEND",	// 3
		"REDIRECT_HEREDOC", // 4
		"PIPE",				// 5
		"AND",				// 6
		"OR",				// 7
		"LPAREN",			// 8
		"RPAREN",			// 9
		"EOF_"				// 10
	};
	if (type >= 0 && type <= sizeof(token_names) / sizeof(token_names[0]))
	{
		return (token_names[type]);
	}
	return ("UNKNOWN");
}

void print_redirects(t_redirect *redir, int depth)
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
void print_ast(t_node *node, int depth)
{
	const char *type;
	t_cmd *cmd;

	if (!node)
		return;
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

void print_tokens(t_token *tokens)
{
	int token_count;

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

int main(int argc, char **argv, char **envp)
{
	char *input;
	t_token *tokens;
	t_token *tokens_copy;
	t_node *ast;

	(void)argc;
	(void)argv;
	(void)envp;
	init_signals();
	while (1)
	{
		input = readline("\001\033[1;32m\002💚 🐚 ms: ➜\001\033[0m\002 ");
		if (!input)
		{
			printf("exit");
			break;
		}
		// TEMPORARY
		if (strcmp(input, "exit") == 0)
		{
			printf("exit");
			return (0);
		}
		//
		if (strlen(input) > 0)
		{
			add_history(input);
			tokens = tokenize(input);
			print_tokens(tokens);
			tokens_copy = tokens;
			ast = parse(&tokens_copy);
			if (!ast)
			{
				printf("\033[1;31mParse error: invalid syntax or memory error\033[0m\n");
				if (tokens)
					free_token_list(tokens);
				free(input);
				continue;
			}
			printf("\n🌳 \033[1;35mAST:\033[0m\n");
			print_ast(ast, 0);
			if (ast)
				free_ast(ast);
			if (tokens)
				free_token_list(tokens);
		}
		free(input);
	}
	return (0);
}
