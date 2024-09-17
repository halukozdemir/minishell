/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:54:36 by halozdem          #+#    #+#             */
/*   Updated: 2023/12/23 16:55:45 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	total;
	void	*memory;

	total = count * size;
	memory = malloc(total);
	if (!memory)
		return (NULL);
	ft_bzero(memory, total);
	if (memory == NULL)
		return (0);
	return (memory);
}
