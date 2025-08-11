#include "minishell.h"

int	handle_input_line(t_shell *shell)
{
	char	*input;

	g_received_signal = 0;
	input = readline("\001\033[1;32m\002🐚 ms: ➜\001\033[0m\002 ");
	shell->heredoc_line++;
	if (g_received_signal == SIGINT)
	{
		shell->last_status = 130;
		if (input)
			free(input);
		return (1);
	}
	if (!input)
	{
		printf("exit\n");
		return (0);
	}
	if (*input == '\0')
	{
		free(input);
		return (1);
	}
	input = handle_multiline_input(input, shell);
	if (!input)
		return (1);
	if (ft_strlen(input) > 0)
		process_input(input, shell);
	free(input);
	return (1);
}

int	process_input(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_node	*ast;

	add_history(input);
	tokens = tokenize(input);
	shell->token = tokens;
	if (validate_and_parse(shell, &tokens, &ast))
		return (1);
	execute_ast(ast, shell);
	cleanup_resources(ast, tokens, shell);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	**my_envp;

	(void)argc;
	(void)argv;
	shell = init_minishell(envp, &my_envp);
	if (!shell)
		return (1);
	while (handle_input_line(shell))
		continue ;
	if (my_envp)
		free_envp(my_envp);
	if (shell)
		free(shell);
	return (0);
}
