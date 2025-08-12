#include "minishell.h"

int	is_binary_op(t_tokens t)
{
	return (t == PIPE || t == AND || t == OR);
}

int	check_lparen_context(t_token *prev, t_token *curr)
{
	if (!prev || curr->type != LPAREN)
		return (0);
	if (prev->type == WORD || prev->type == SINGLE_QUOTED
		|| prev->type == DOUBLE_QUOTED)
		return (syntax_error("("), 1);
	if (prev->type == REDIRECT_IN || prev->type == REDIRECT_OUT
		|| prev->type == REDIRECT_HEREDOC || prev->type == REDIRECT_APPEND)
		return (syntax_error("("), 1);
	return (0);
}

int	check_empty_paren(t_token *curr)
{
	if (curr->type == LPAREN && curr->next && curr->next->type == RPAREN)
		return (syntax_error(")"), 1);
	return (0);
}

int	check_binary_sequence(t_token *prev, t_token *curr)
{
	if (prev && is_binary_op(curr->type) && is_binary_op(prev->type))
		return (syntax_error(curr->final_value), 1);
	return (0);
}

int	check_redirect_delimiter(t_token *curr)
{
	if (curr->type == REDIRECT_HEREDOC || curr->type == REDIRECT_IN
		|| curr->type == REDIRECT_OUT || curr->type == REDIRECT_APPEND)
	{
		if (!curr->next)
			return (syntax_error("newline"), 1);
		if (!(curr->next->type == WORD || curr->next->type == SINGLE_QUOTED
				|| curr->next->type == DOUBLE_QUOTED))
			return (syntax_error(curr->next->final_value), 1);
	}
	return (0);
}
