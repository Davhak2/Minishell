#include "libft.h"

static size_t	word_count(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static int	malloc_words(char **dest, size_t index, size_t size)
{
	dest[index] = (char *)malloc(size);
	if (!dest[index])
	{
		while (index--)
			free(dest[index]);
		free(dest);
		return (1);
	}
	return (0);
}

static int	fill_array(char **dest, char const *s, char c)
{
	size_t	size;
	size_t	index;

	index = 0;
	while (*s)
	{
		size = 0;
		while (*s && *s == c)
			s++;
		while (*s && *s != c)
		{
			s++;
			size++;
		}
		if (size)
		{
			if (malloc_words(dest, index, size + 1))
				return (1);
			ft_strlcpy(dest[index], s - size, size + 1);
			index++;
		}
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	wordcount;

	wordcount = word_count(s, c);
	if (!s)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (wordcount + 1));
	if (!result)
		return (NULL);
	result[wordcount] = NULL;
	if (fill_array(result, s, c))
		return (NULL);
	return (result);
}
