/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:31:40 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 17:31:40 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	pwd(void)
{
	char	wd[512];

	if (!getcwd(wd, 512))
		return (EXIT_FAILURE);
	write(1, wd, ft_strlen(wd));
	write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
