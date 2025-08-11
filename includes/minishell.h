/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:31:04 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 15:31:04 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stddef.h>
# include <dirent.h>
# include <fcntl.h>
# include <errno.h>

# include "libft.h"
# include "structs.h"
# include "builtins.h"
# include "executor.h"
# include "expander.h"
# include "lexer.h"
# include "parser.h"
# include "signals.h"
# include "utils.h"

#endif // MINISHELL_H
