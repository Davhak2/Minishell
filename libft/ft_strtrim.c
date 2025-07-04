#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ptr;
	size_t	start;
	size_t	end;
	size_t	length;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	length = end - start;
	ptr = (char *)malloc(length + 1);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, &s1[start], length);
	ptr[length] = '\0';
	return (ptr);
}
