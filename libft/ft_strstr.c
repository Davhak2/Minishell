/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letto <letto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:39:54 by letto             #+#    #+#             */
/*   Updated: 2025/08/10 15:40:01 by letto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	needle_len;

	if (!*needle)
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	while (*haystack)
	{
		if (!ft_strncmp(haystack, needle, needle_len))
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
