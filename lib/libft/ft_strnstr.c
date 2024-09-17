/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:03:08 by halozdem          #+#    #+#             */
/*   Updated: 2023/12/23 16:44:17 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str1, const char *str2, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!str1 && !len)
		return (NULL);
	if (str2 == str1 || str2[0] == 0)
		return ((char *)str1);
	while (str1[i] != 0 && i < len)
	{
		if (str1[i] == str2[j])
		{
			while (str1[i + j] == str2[j] && i + j < len)
			{
				if (str2[j + 1] == 0)
					return ((char *)str1 + i);
				j++;
			}
			j = 0;
		}
		i++;
	}
	return (0);
}
