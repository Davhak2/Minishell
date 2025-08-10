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

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>

typedef enum e_tokens
{
	WORD,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC,
	PIPE,
	AND,
	OR,
	LPAREN,
	RPAREN,
	EOF_,
	SINGLE_QUOTED,
	DOUBLE_QUOTED,
}						t_tokens;

typedef struct s_token
{
	t_tokens			type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_type
{
	t_tokens			type;
	char				*value;
}						t_type;

typedef struct s_node
{
	t_tokens			type;
	char				*value;
	struct s_node		*left;
	struct s_node		*right;
}						t_node;

typedef struct s_redirect
{
	t_tokens			type;
	char				*filename;
	char				*heredoc_filename;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_cmd
{
	char				*cmd;
	char				**args;
	t_tokens			*arg_types;
	t_redirect			*redirects;
}						t_cmd;

typedef struct s_shell
{
	t_node				*node;
	t_redirect			*redir;
	t_token				*token;
	char				***envp;
	int					last_status;
	int					heredoc_line;
	int					stdin_backup;
	int					stdout_backup;
}						t_shell;

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
