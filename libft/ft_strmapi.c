/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luminous <luminous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:41:32 by luminous          #+#    #+#             */
/*   Updated: 2025/07/04 16:41:33 by luminous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*res;
	char	*ptr;
	size_t	i;

	ptr = (char *)s;
	res = (char *)malloc(ft_strlen(s) + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (ptr[i])
	{
		res[i] = f(i, ptr[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}
