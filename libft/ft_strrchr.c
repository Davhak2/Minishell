/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luminous <luminous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:41:42 by luminous          #+#    #+#             */
/*   Updated: 2025/07/04 16:41:43 by luminous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char			*offset;
	unsigned char	target;
	size_t			length;

	length = ft_strlen(s);
	target = (unsigned char)c;
	offset = (char *)s;
	if (target == '\0')
		return (offset + length);
	while (length-- > 0)
	{
		if (offset[length] == target)
			return (offset + length);
	}
	return (NULL);
}
