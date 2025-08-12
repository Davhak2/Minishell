/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:05:59 by letto             #+#    #+#             */
/*   Updated: 2025/08/12 15:47:04 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

int		handle_quotes(t_token **list, char **ptr);
int		read_operator(t_type *type, char **ptr);
int		handle_word(t_token **list, char **ptr);
int		dispatch_token(t_token **list, char **ptr);
int		append_slice(char **dest, const char *start, size_t len);
int		read_quoted(char **p, t_tokens *qt, char **res);
int		check_word_token(t_token *list);

char		*process_quotes(char **ptr, t_tokens *quote_type, t_segment **segments);
t_segment	*create_segment(t_tokens type, char *value);
void		add_segment(t_segment **head, t_segment *new_segment);
int			handle_quoted(char **ptr, t_segment **segments,
			char **result, int *has_segments);
void		scan_mixed_lookahead(char *ptr, int *has_word, int *has_quote);

#endif // LEXER_H
