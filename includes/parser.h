/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:31:09 by letto             #+#    #+#             */
/*   Updated: 2025/08/11 21:13:37 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

t_token	*tokenize(char *line);
int		validate_syntax(t_token *tokens);

t_node	*parse_or(t_token **list);
t_node	*parse_and(t_token **list);
t_node	*parse_pipe(t_token **list);
t_node	*parse_parenthesis(t_token **list);
t_node	*parse(t_token **list);
t_node	*simple_command(t_token **list);

int		scan_and_collect(t_token *cur, t_token **end, t_redirect **rh);
int		count_args(t_token *cur);
int		alloc_arrays(int argc, char ***argv, t_tokens **types);
int		fill_args(t_token *cur, char **argv, t_tokens *types);

int		is_binary_op(t_tokens t);
int		check_lparen_context(t_token *prev, t_token *curr);
int		check_empty_paren(t_token *curr);
int		check_binary_sequence(t_token *prev, t_token *curr);
int		check_redirect_delimiter(t_token *curr);

int		validate_syntax_loop(t_token *curr, t_token *prev, int *paren_depth);
int		update_paren_depth(t_token *curr, int *paren_depth);
int		check_trailing_op_or_redir(t_token *curr);



#endif // PARSER_H
