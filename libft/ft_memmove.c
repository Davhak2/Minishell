/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luminous <luminous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:40:52 by luminous          #+#    #+#             */
/*   Updated: 2025/07/04 16:40:53 by luminous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*dest_offset;
	char	*src_offset;

	dest_offset = (char *)dest;
	src_offset = (char *)src;
	if (dest == src || n == 0)
		return (dest);
	if (dest >= src && dest <= src + n)
	{
		while (n--)
			dest_offset[n] = src_offset[n];
	}
	else
	{
		while (n--)
			*(dest_offset)++ = *(src_offset)++;
	}
	return (dest);
}
