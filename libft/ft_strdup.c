/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luminous <luminous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:41:17 by luminous          #+#    #+#             */
/*   Updated: 2025/07/04 16:41:18 by luminous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*copy;
	size_t	size;

	size = ft_strlen(s);
	copy = (char *)malloc(size + 1);
	if (!copy)
		return (0);
	size = 0;
	while (s[size])
	{
		copy[size] = s[size];
		size++;
	}
	copy[size] = '\0';
	return (copy);
}
