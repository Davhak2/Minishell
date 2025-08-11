/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:30:07 by letto             #+#    #+#             */
/*   Updated: 2025/08/11 21:16:40 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_paren(char op, t_type *type)
{
	if (op == '(')
	{
		type->type = LPAREN;
		type->value = ft_strdup("(");
	}
	else if (op == ')')
	{
		type->type = RPAREN;
		type->value = ft_strdup(")");
	}
}

static t_type	set_single_op(char op)
{
	t_type	type;

	type.type = WORD;
	type.value = NULL;
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
	else if (op == '(' || op == ')')
		set_paren(op, &type);
	return (type);
}

static t_type	set_double_op(char op)
{
	t_type	type;

	type.type = WORD;
	type.value = NULL;
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
	return (type);
}

t_type	set_type(char op, bool mode)
{
	if (mode)
		return (set_single_op(op));
	return (set_double_op(op));
}
