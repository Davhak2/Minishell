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
