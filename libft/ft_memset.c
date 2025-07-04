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
