/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:30:50 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 18:20:16 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "parser.h"

int		ft_echo(char **args, t_shell *shell);

int		ft_cd(char **args, t_shell *shell);
int		cd_home(t_shell *shell);
int		update_pwd_vars(char *cwd, t_shell *shell);
char	*cwd_with_fallback(char **args, t_shell *shell);
int		cd_oldpwd(t_shell *shell);
char	*path_join_fallback(char *base, char *rel);

int		ft_pwd(char **envp);

int		ft_export(char **args, t_shell *shell);
char	**update_env_var(char **envp, char *new_var, int index);
char	*get_var_name(char *str);
char	**update_or_add_env_var(char **envp, char *new_var);
char	**add_env_var(char **envp, char *new_var);
char	*create_second_var(char *str);
int		is_valid_second_var(char *str);

int		ft_unset(char **args, t_shell *shell);
int		ft_env(char **args, char **envp);
int		ft_exit(char **args, t_shell *shell);

#endif // BUILTINS_H
