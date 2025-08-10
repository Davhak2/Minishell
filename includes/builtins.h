/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:30:50 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 15:30:51 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "parser.h"

int	ft_echo(char **args, t_shell *shell);
int	ft_cd(char **args, t_shell *shell);
int	ft_pwd(char **envp);
int	ft_export(char **args, t_shell *shell);
int	ft_unset(char **args, t_shell *shell);
int	ft_env(char **args, char **envp);
int	ft_exit(char **args, t_shell *shell);

#endif // BUILTINS_H
