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
