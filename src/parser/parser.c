#include "libft.h"
#include "parser.h"

t_node	*simple_command(t_token **list)
{
	int			argc;
	t_token		*cur;
	char		**argv;
	int			i;
	t_cmd		*cmd;
	t_node		*node;
	t_redirect	*redir_head;
	t_redirect	*redir;
	t_tokens	*arg_types;

	argc = 0;
	cur = *list;
	redir_head = NULL;
	while (cur && (cur->type == WORD || cur->type == SINGLE_QUOTED
			|| cur->type == REDIRECT_IN || cur->type == REDIRECT_OUT
			|| cur->type == REDIRECT_HEREDOC || cur->type == REDIRECT_APPEND))
	{
		if (cur->type == WORD || cur->type == SINGLE_QUOTED)
			argc++;
		else
		{
			redir = malloc(sizeof(t_redirect));
			if (!redir)
			{
				free_redirects(redir_head);
				return (NULL);
			}
			redir->type = cur->type;
			cur = cur->next;
			if (!cur || (cur->type != WORD && cur->type != SINGLE_QUOTED))
			{
				free_redirects(redir_head);
				return (NULL);
			}
			redir->filename = ft_strdup(cur->value);
			redir->next = redir_head;
			redir_head = redir;
		}
		cur = cur->next;
	}
	argv = malloc(sizeof(char *) * (argc + 1));
	arg_types = malloc(sizeof(t_tokens) * argc);
	if (!argv || !arg_types)
	{
		free(argv);
		free(arg_types);
		free_redirects(redir_head);
		return (NULL);
	}
	cur = *list;
	i = 0;
	while (cur && (cur->type == WORD || cur->type == SINGLE_QUOTED
			|| cur->type == REDIRECT_IN || cur->type == REDIRECT_OUT
			|| cur->type == REDIRECT_HEREDOC || cur->type == REDIRECT_APPEND))
	{
		if (cur->type == WORD || cur->type == SINGLE_QUOTED)
		{
			argv[i] = ft_strdup(cur->value);
			arg_types[i] = cur->type;
			i++;
		}
		cur = cur->next;
	}
	argv[i] = NULL;
	if (argc == 0)
	{
		free(argv);
		free(arg_types);
		free_redirects(redir_head);
		return (NULL);
	}
	cmd = malloc(sizeof(t_cmd));
	node = malloc(sizeof(t_node));
	if (!cmd || !node)
	{
		free(argv);
		free(arg_types);
		free(cmd);
		free(node);
		free_redirects(redir_head);
		return (NULL);
	}
	cmd->cmd = argv[0];
	cmd->args = argv;
	cmd->arg_types = arg_types;
	cmd->redirects = redir_head;
	node->type = WORD;
	node->value = (char *)cmd;
	node->left = NULL;
	node->right = NULL;
	*list = cur;
	return (node);
}

t_node	*parse_or(t_token **list)
{
	t_node	*left;
	t_node	*right;
	t_node	*node;

	left = parse_and(list);
	while (*list && (*list)->type == OR)
	{
		*list = (*list)->next;
		right = parse_and(list);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		node = malloc(sizeof(t_node));
		if (!node)
		{
			free_ast(left);
			free_ast(right);
			return (NULL);
		}
		node->type = OR;
		node->left = left;
		node->right = right;
		node->value = NULL;
		left = node;
	}
	return (left);
}

t_node	*parse_and(t_token **list)
{
	t_node	*left;
	t_node	*right;
	t_node	*node;

	left = parse_pipe(list);
	while (*list && (*list)->type == AND)
	{
		*list = (*list)->next;
		right = parse_pipe(list);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		node = malloc(sizeof(t_node));
		if (!node)
		{
			free_ast(left);
			free_ast(right);
			return (NULL);
		}
		node->type = AND;
		node->left = left;
		node->right = right;
		node->value = NULL;
		left = node;
	}
	return (left);
}

t_node	*parse_pipe(t_token **list)
{
	t_node	*left;
	t_node	*right;
	t_node	*node;

	left = parse_parenthesis(list);
	while (*list && (*list)->type == PIPE)
	{
		*list = (*list)->next;
		right = parse_parenthesis(list);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		node = malloc(sizeof(t_node));
		if (!node)
		{
			free_ast(left);
			free_ast(right);
			return (NULL);
		}
		node->type = PIPE;
		node->left = left;
		node->right = right;
		node->value = NULL;
		left = node;
	}
	return (left);
}

t_node	*parse_parenthesis(t_token **list)
{
	t_node	*node;

	if (*list && (*list)->type == LPAREN)
	{
		*list = (*list)->next;
		node = parse_or(list);
		if (!node)
			return (NULL);
		if (!(*list) || (*list)->type != RPAREN)
		{
			free_ast(node);
			return (NULL);
		}
		*list = (*list)->next;
		return (node);
	}
	return (simple_command(list));
}

t_node	*parse(t_token **list)
{
	return (parse_or(list));
}
