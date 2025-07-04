#include "libft.h"

static size_t	digitcount(int n)
{
	size_t	size;

	size = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		size++;
		n *= -1;
	}
	while (n)
	{
		size++;
		n /= 10;
	}
	return (size);
}

static void	fill(char *res, int n, int index)
{
	if (n < 0)
	{
		res[0] = '-';
		n *= -1;
	}
	res[index--] = '\0';
	while (n)
	{
		res[index--] = (n % 10) + '0';
		n /= 10;
	}
}

char	*ft_itoa(int n)
{
	char	*res;
	size_t	digit_count;

	digit_count = digitcount(n);
	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	res = (char *)malloc(digit_count + 1);
	if (!res)
		return (NULL);
	fill(res, n, digit_count);
	return (res);
}
