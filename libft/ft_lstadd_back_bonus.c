/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luminous <luminous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:40:21 by luminous          #+#    #+#             */
/*   Updated: 2025/07/04 16:40:22 by luminous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
