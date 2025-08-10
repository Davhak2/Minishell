/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:55:39 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 18:00:35 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

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
