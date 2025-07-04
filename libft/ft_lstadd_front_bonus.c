#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *added)
{
	added->next = *lst;
	*lst = added;
}
