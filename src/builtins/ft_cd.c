#include "builtins.h"
#include "expander.h"
#include "libft.h"
#include <unistd.h>

int	ft_cd(char **args, char **envp)
{
}

/* TODO:
Обработать случай, когда аргумент отсутствует (переход в $HOME).
Обновить OLDPWD и PWD в envp после успешного перехода.
Добавить обработку ошибок (например, если директория не существует).
*/