/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:31:00 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 17:26:27 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "parser.h"

void	expand_ast(t_node *node, char **envp, t_shell *shell);
char	*get_env_value(const char *name, char **envp);
char	*expand_tilde(char *arg, char **envp);

#endif
