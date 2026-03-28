#include <limits.h>
#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	value;

	sign = 1;
	value = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign *= -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		value *= 10;
		value += *nptr -48;
		nptr++;
	}
	return (value * sign);
}

int	ft_strtoi(const char *nptr, const char** endptr)
{
	int		sign;
	long	value;

	sign = 1;
	value = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign *= -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		if (value * 10 + *nptr - 48 > INT_MAX - (sign == -1))
			break;
		value *= 10;
		value += *nptr -48;
		nptr++;
	}
	if (endptr)
		*endptr = nptr;
	return (value * sign);
}
