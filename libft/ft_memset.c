/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luminous <luminous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:40:54 by luminous          #+#    #+#             */
/*   Updated: 2025/07/04 16:40:55 by luminous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int c, size_t n)
{
	char	*offset;
	size_t	i;

	offset = (char *)dest;
	i = 0;
	while (i++ < n)
		*offset++ = (unsigned char)c;
	return (dest);
}
