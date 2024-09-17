/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 18:10:40 by halozdem          #+#    #+#             */
/*   Updated: 2023/12/23 16:43:57 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = malloc((((ft_strlen(s1)) + (ft_strlen(s2))) * sizeof(char)) + 1);
	if (!str)
		return (NULL);
	while (s1[i] || s2[j])
	{
		if (s1[i])
		{
			str[i] = s1[i];
			i++;
		}
		else
		{
			str[i + j] = s2[j];
			j++;
		}
	}
	str[i + j] = 0;
	return (str);
}
