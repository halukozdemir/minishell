/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:51:43 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 16:51:43 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	cd(char *path, t_env *env)
{
	if (!path)
	{
		path = get_env_value(env, "HOME");
		if (!path)
			return (EXIT_FAILURE);
	}
	if (chdir(path) == -1)
	{
		g_exit_status = 1;
		perror("cd");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
