/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:48:53 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 16:48:53 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env(t_env *env)
{
	t_env	*env_list;

	env_list = env;
	while (env_list)
	{
		write(1, env_list->key, ft_strlen(env_list->key));
		write(1, "=", 1);
		write(1, env_list->value, ft_strlen(env_list->value));
		write(1, "\n", 1);
		env_list = env_list->next;
	}
}
