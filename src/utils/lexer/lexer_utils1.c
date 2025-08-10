/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:18:10 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 16:59:11 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

void	create_and_add(t_token **list, t_type type)
{
	t_token	*token;

	token = create_token(type.type, type.value);
	if (!token)
	{
		free(type.value);
		return ;
	}
	add_token_to_list(list, token);
}

void	syntax_exit(char ptr, t_token *list)
{
	printf("minishell: syntax error near unexpected token `%c'\n", ptr);
	free_token_list(list);
}
