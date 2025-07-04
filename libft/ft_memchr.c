/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luminous <luminous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:40:45 by luminous          #+#    #+#             */
/*   Updated: 2025/07/04 16:40:46 by luminous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;
	unsigned char	target;

	target = (unsigned char)c;
	ptr = (unsigned char *)s;
	while (n-- > 0)
	{
		if (*ptr == target)
			return ((void *)(ptr));
		ptr++;
	}
	return (NULL);
}
