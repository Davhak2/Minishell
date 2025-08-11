/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:31:09 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 17:05:51 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

t_token					*tokenize(char *line);
int						validate_syntax(t_token *tokens);
t_node					*parse_or(t_token **list);
t_node					*parse_and(t_token **list);
t_node					*parse_pipe(t_token **list);
t_node					*parse_parenthesis(t_token **list);
t_node					*parse(t_token **list);
t_node					*simple_command(t_token **list);
int						scan_and_collect(t_token *cur, t_token **end,
							t_redirect **rh);
int						count_args(t_token *cur);
int						alloc_arrays(int argc, char ***argv, t_tokens **types);
int						fill_args(t_token *cur, char **argv, t_tokens *types);

#endif // PARSER_H
