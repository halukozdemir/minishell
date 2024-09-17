/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 15:20:07 by halozdem          #+#    #+#             */
/*   Updated: 2023/12/23 16:56:04 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long int	leng(long a)
{
	int	count;

	count = 0;
	if (a < 0)
		count = 1;
	while (a >= 10 || a <= -10)
	{
		a = a / 10;
		count++;
	}
	count++;
	return (count);
}

static char	*convert(char *s, long number, long len)
{
	s[len] = '\0';
	if (number == 0)
	{
		s[--len] = '0';
		return (s);
	}
	while (number > 0)
	{
		s[--len] = 48 + (number % 10);
		number = number / 10;
	}
	return (s);
}

char	*ft_itoa(int n)
{
	long	nb;
	long	len;
	char	*str;

	nb = 0;
	len = leng(n);
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	if (n == 0)
		str[0] = '0';
	else if (n < 0)
	{
		nb = (((long)n) * -1);
		str[0] = '-';
	}
	else
		nb = n;
	str = convert(str, nb, len);
	return (str);
}
