#include "minishell.h"

const char	*get_token_type_name(t_tokens type)
{
	static const char *token_names[] = {
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
		"EOF_"              // 10
	};
	if (type >= 0 && type < sizeof(token_names) / sizeof(token_names[0]))
		return (token_names[type]);
	return ("UNKNOWN");
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

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	init_signals();

	while (1)
	{
		char *input = readline("\001\033[1;32m\002💚 🐚 niGGol ➜\001\033[0m\002 ");
		if (!input)
			break ;

		// TEMPORARY
		if (strcmp(input, "exit") == 0)
			return (0);
		//

		if (strlen(input) > 0)
		{
			add_history(input);

			t_token *tokens = tokenize(input);
			print_tokens(tokens);

			free_token_list(tokens);
		}

		free(input);
	}
	return (0);
}