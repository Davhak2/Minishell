/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:05:59 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 17:06:17 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

int	handle_quotes(t_token **list, char **ptr);
int	read_operator(t_type *type, char **ptr);
int	handle_word(t_token **list, char **ptr);
int	dispatch_token(t_token **list, char **ptr);
int	append_slice(char **dest, const char *start, size_t len);
int	read_quoted(char **p, t_tokens *qt, char **res);

#endif // LEXER_H
