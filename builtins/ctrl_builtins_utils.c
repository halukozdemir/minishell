/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_builtins_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:53:26 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/20 21:54:24 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	handle_pwd(t_jobs *jobs)
{
	char	state;

	state = pwd();
	jobs->mshell->doll_quest = state;
	return (state);
}

char	handle_cd(t_jobs *jobs, char *arg, t_env *env)
{
	char	state;

	state = cd(jobs, arg, env);
	jobs->mshell->doll_quest = state;
	return (state);
}

char	handle_echo(t_jobs *jobs, t_job *job)
{
	echo(job);
	jobs->mshell->doll_quest = 0;
	return (EXIT_SUCCESS);
}

char	handle_env(t_jobs *jobs)
{
	env(jobs->env);
	jobs->mshell->doll_quest = 0;
	return (EXIT_SUCCESS);
}

char	handle_exit(t_jobs *jobs, char **args)
{
	exit_d(jobs, args);
	return (EXIT_SUCCESS);
}
