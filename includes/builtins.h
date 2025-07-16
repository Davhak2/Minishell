#ifndef BUILTINS_H
# define BUILTINS_H

// Prototypes for built-in commands
# include "minishell.h"

// TODO: global exit status handling

int	ft_echo(char **args);
int	ft_cd(char **args, char **envp);
int	ft_pwd(char **envp);
int	ft_export(char **args, char **envp);
int	ft_unset(char **args, char **envp);
int	ft_env(char **envp);
int	ft_exit(char **args);

#endif // BUILTINS_H
