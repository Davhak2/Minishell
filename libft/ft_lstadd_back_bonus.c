#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *added)
{
	t_list	*last;

	if (!lst || !added)
		return ;
	if (!*lst)
	{
		*lst = added;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = added;
}
