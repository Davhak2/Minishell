/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luminous <luminous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:41:36 by luminous          #+#    #+#             */
/*   Updated: 2025/07/04 16:41:37 by luminous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*s1_offset;
	unsigned char	*s2_offset;

	s1_offset = (unsigned char *)s1;
	s2_offset = (unsigned char *)s2;
	while (n-- > 0)
	{
		if (*s1_offset - *s2_offset != 0)
			return (*s1_offset - *s2_offset);
		else if (*s1_offset == '\0' && *s2_offset == '\0')
			return (0);
		s1_offset++;
		s2_offset++;
	}
	return (0);
}
