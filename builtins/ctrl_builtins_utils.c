/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_builtins_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:53:26 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 16:58:06 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	handle_pwd(void)
{
	char	state;

	state = pwd();
	g_exit_status = state;
	return (state);
}

char	handle_cd(char *arg)
{
	char	state;

	state = cd(arg);
	g_exit_status = state;
	return (state);
}

char	handle_echo(t_job *job)
{
	echo(job);
	g_exit_status = 0;
	return (EXIT_SUCCESS);
}

char	handle_env(t_env *envv)
{
	env(envv);
	g_exit_status = 0;
	return (EXIT_SUCCESS);
}

char	handle_exit(char **args)
{
	exit_d(args);
	return (EXIT_SUCCESS);
}
