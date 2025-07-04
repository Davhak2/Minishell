#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	destsize;
	size_t	i;

	destsize = ft_strlen(dest);
	i = 0;
	if (size <= destsize)
		return (size + ft_strlen(src));
	while (src[i] != '\0' && destsize + i + 1 < size)
	{
		dest[destsize + i] = src[i];
		i++;
	}
	dest[destsize + i] = '\0';
	return (destsize + ft_strlen(src));
}
