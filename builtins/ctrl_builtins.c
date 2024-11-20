/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/11/16 16:51:55 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 16:51:55 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_builtin(t_job *job)
{
	if (job->args && !ft_strncmp(job->args[0], "pwd", 3) && ft_strlen(job->args[0]) == 3)
		job->is_builtin = true;
	else if (job->args && !ft_strncmp(job->args[0], "cd", 2)
		&& ft_strlen(job->args[0]) == 2)
		job->is_builtin = true;
	else if (job->args && !ft_strncmp(job->args[0], "echo", 4)
		&& ft_strlen(job->args[0]) == 4)
		job->is_builtin = true;
	else if (job->args && !ft_strncmp(job->args[0], "env", 3)
		&& ft_strlen(job->args[0]) == 3)
		job->is_builtin = true;
	else if (job->args && !ft_strncmp(job->args[0], "exit", 4)
		&& ft_strlen(job->args[0]) == 4)
		job->is_builtin = true;
	else if (job->args && !ft_strncmp(job->args[0], "unset", 5)
		&& ft_strlen(job->args[0]) == 5)
		job->is_builtin = true;
	else if (job->args && !ft_strncmp(job->args[0], "export", 6)
		&& ft_strlen(job->args[0]) == 6)
		job->is_builtin = true;
	else
		job->is_builtin = false;
}

char	ctrl_builtins(t_jobs *jobs, t_job *job)
{
	if (!ft_strncmp(job->args[0], "pwd", 3) && ft_strlen(job->args[0]) == 3)
		return (handle_pwd(jobs));
	else if (!ft_strncmp(job->args[0], "cd", 2)
		&& ft_strlen(job->args[0]) == 2)
		return (handle_cd(jobs, job->args[1], jobs->env));
	else if (!ft_strncmp(job->args[0], "echo", 4)
		&& ft_strlen(job->args[0]) == 4)
		return (handle_echo(jobs, job));
	else if (!ft_strncmp(job->args[0], "env", 3)
		&& ft_strlen(job->args[0]) == 3)
		return (handle_env(jobs));
	else if (!ft_strncmp(job->args[0], "exit", 4)
		&& ft_strlen(job->args[0]) == 4)
		return (handle_exit(jobs, job->args));
	else if (!ft_strncmp(job->args[0], "unset", 5)
		&& ft_strlen(job->args[0]) == 5)
		return (handle_unset(&jobs->env, job->args));
	else if (!ft_strncmp(job->args[0], "export", 6)
		&& ft_strlen(job->args[0]) == 6)
		return (handle_export(jobs->env, job->args));
	return (-1);
}
