/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:22:14 by halozdem          #+#    #+#             */
/*   Updated: 2023/12/23 16:43:50 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	size;
	char	*dest;

	size = ft_strlen(s1);
	dest = malloc(size * sizeof(char) + 1);
	if (!dest)
		return (NULL);
	ft_memcpy(dest, s1, size);
	dest[size] = 0;
	return (dest);
}
