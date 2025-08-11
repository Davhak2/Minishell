/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:55:39 by letto             #+#    #+#             */
/*   Updated: 2025/08/11 21:17:02 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_arg_token(t_tokens t)
{
	return (t == WORD || t == SINGLE_QUOTED || t == DOUBLE_QUOTED);
}

int	is_redir_token(t_tokens t)
{
	return (t == REDIRECT_IN || t == REDIRECT_OUT || t == REDIRECT_HEREDOC
		|| t == REDIRECT_APPEND);
}

int	in_simple_span(t_tokens t)
{
	return (is_arg_token(t) || is_redir_token(t));
}
