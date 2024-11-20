/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:47:36 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/20 18:13:32 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_cmd(t_jobs *jobs, t_job *job)
{
	char	*env_path;
	char	*exec_path;

	env_path = get_env_value(jobs->env, "PATH");
	if (!env_path)
	{
		handle_no_env_path(jobs, job);
		return ;
	}
	if (ft_strchr(job->args[0], '/'))
	{
		handle_execution_error(job->args[0]);
		exec_path = job->args[0];
	}
	else
		exec_path = find_path(env_path, job->args[0]);
	if (!exec_path)
		handle_exec_path_error(job);
	g_exit_status = 0;
	execve(exec_path, job->args, env_to_double_pointer(jobs->env));
	exit(127);
}
