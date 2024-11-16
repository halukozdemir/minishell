/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_builtins_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:57:40 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 16:58:08 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	handle_unset(t_env **env, char **args)
{
	unset(env, args);
	return (EXIT_SUCCESS);
}

char	handle_export(t_env *env, char **args)
{
	char	state;

	state = export(env, args);
	return (state);
}
