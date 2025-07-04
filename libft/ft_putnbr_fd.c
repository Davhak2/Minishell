#include "libft.h"

static size_t	digitcount(int n)
{
	size_t	size;

	size = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		size++;
		n /= 10;
	}
	return (size);
}

static void	case_handler(int *n, int *fd)
{
	if (*n == 0)
	{
		write(*fd, "0", 1);
		return ;
	}
	if (*n == -2147483648)
	{
		write(*fd, "-2147483648", 11);
		return ;
	}
	if (*n < 0)
	{
		write(*fd, "-", 1);
		*n *= -1;
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	char	c;
	size_t	digits;
	size_t	i;
	size_t	tmp;

	case_handler(&n, &fd);
	if (n == 0 || n == -2147483648)
		return ;
	digits = digitcount(n);
	tmp = digits;
	i = 1;
	while (--tmp)
		i *= 10;
	while (n || digits)
	{
		c = (n / i) + '0';
		write(fd, &c, 1);
		n %= i;
		i /= 10;
		digits--;
	}
}
