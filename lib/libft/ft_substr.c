/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:35:57 by halozdem          #+#    #+#             */
/*   Updated: 2023/12/23 16:55:27 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*str;
	unsigned int	i;

	i = 0;
	if (!s)
		return (0);
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	if ((size_t)start > ft_strlen(s))
		return (ft_strdup(""));
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (0);
	while (i < len && s[start + i != 0])
	{
		str[i] = *(s + start + i);
		i++;
	}
	str[i] = 0;
	return (str);
}
