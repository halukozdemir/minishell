/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:28:55 by halozdem          #+#    #+#             */
/*   Updated: 2023/12/18 13:51:03 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;
	char	*str;
	int		m;
	int		k;

	len = ft_strlen(s1);
	if (!s1)
		return (NULL);
	if (!set)
		return ((char *)s1);
	i = 0;
	while (s1[i] != '\0' && ft_strchr(set, s1[i]))
		i++;
	while (len > i + 1 && ft_strchr(set, s1[len - 1]))
		len--;
	str = ft_calloc((len - i + 1), sizeof(char));
	if (!str)
		return (0);
	m = len - i;
	k = 0;
	while (k < m)
		str[k++] = s1[i++];
	return (str);
}
