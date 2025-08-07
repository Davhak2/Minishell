#include "utils.h"
#include "libft.h"

t_type	set_type(char op, bool mode)
{
	t_type	type;

	type.type = WORD;
	type.value = NULL;
	if (mode)
	{
		if (op == '|')
		{
			type.type = PIPE;
			type.value = ft_strdup("|");
		}
		else if (op == '<')
		{
			type.type = REDIRECT_IN;
			type.value = ft_strdup("<");
		}
		else if (op == '>')
		{
			type.type = REDIRECT_OUT;
			type.value = ft_strdup(">");
		}
		else if (op == '(')
		{
			type.type = LPAREN;
			type.value = ft_strdup("(");
		}
		else if (op == ')')
		{
			type.type = RPAREN;
			type.value = ft_strdup(")");
		}
		// else if (op == '&') // ?????
		// {
		// 	type.type = WORD;
		// 	type.value = ft_strdup("&");
		// }
	}
	else 
	{
		if (op == '<')
		{
			type.type = REDIRECT_HEREDOC;
			type.value = ft_strdup("<<");
		}
		else if (op == '>')
		{
			type.type = REDIRECT_APPEND;
			type.value = ft_strdup(">>");
		}
		else if (op == '|')
		{
			type.type = OR;
			type.value = ft_strdup("||");
		}
		else if (op == '&')
		{
			type.type = AND;
			type.value = ft_strdup("&&");
		}
	}
	return (type);
}

void	create_and_add(t_token **list, t_type type)
{
	t_token *token;
	token = create_token(type.type, type.value);
	if (!token)
	{
		free(type.value);
		return ;
	}
	add_token_to_list(list, token);
}