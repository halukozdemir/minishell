/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:05:47 by halozdem          #+#    #+#             */
/*   Updated: 2023/12/23 16:16:25 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int c, size_t len)
{
	int				i;
	unsigned char	*ptr;

	ptr = (unsigned char *)dest;
	i = 0;
	while (len--)
	{
		ptr[i] = (char)c;
		i++;
	}
	return (ptr);
}
