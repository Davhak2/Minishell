/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:29:03 by ganersis          #+#    #+#             */
/*   Updated: 2025/08/12 16:03:27 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

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
	SUBSHELL,
	EOF_,
	SINGLE_QUOTED,
	DOUBLE_QUOTED,
}						t_tokens;

typedef struct s_segment
{
	t_tokens			type;
	char				*value;
	struct s_segment	*next;
}						t_segment;

typedef struct s_token
{
	t_tokens			type;
	char				*final_value;
	t_segment			*segments;
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
	int					subshell_depth;
}						t_shell;

typedef struct s_redirect_state
{
	int					pipefd[2];
	int					has_pipe;
}						t_redirect_state;

typedef struct s_expand
{
	char				**result;
	size_t				*j;
	char				**envp;
	int					last_status;
}						t_expand;

typedef struct s_size
{
	char				**envp;
	int					last_status;
}						t_size;

typedef struct s_process
{
	char				***result;
	int					*count;
	int					*cap;
	const char			*dir_path;
}						t_process;

typedef struct s_quote_data
{
	t_segment	**segments;
	char		**result;
	char		*temp_value;
	char		quote_char;
	int			*has_segments;
}						t_quote_data;

#endif // STRUCTS_H
