/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:34:26 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 16:36:00 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_stat_error(char *file_i)
{
	t_stat	stat_t;

	stat(file_i, &stat_t);
	if (S_ISDIR(stat_t.st_mode))
	{
		print_error_message(file_i, ": Is a directory\n");
		exit(1);
	}
	else
	{
		print_error_message(file_i, ": No such file or directory\n");
		exit(1);
	}
}

char	redir_error(t_jobs *jobs, t_job *job, char *file_i, int fd)
{
	t_stat	stat_t;

	if (fd == -1)
	{
		g_exit_status = 1;
		if (jobs->len != 1 || job->is_builtin == false)
			handle_stat_error(file_i);
		else
		{
			stat(file_i, &stat_t);
			if (S_ISDIR(stat_t.st_mode))
				print_error_message(file_i, ": Is a directory\n");
			else
				print_error_message(file_i, ": No such file or directory\n");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

char	file_control(t_jobs *jobs, t_job *job, char *file, int fd)
{
	if (!file)
		return (EXIT_FAILURE);
	if (access(file, F_OK))
	{
		access_error(file, ": No such file or directory\n");
		return (EXIT_FAILURE);
	}
	if (access(file, R_OK))
	{
		access_error(file, ": Permission denied\n");
		exit(1);
	}
	if (redir_error(jobs, job, file, fd))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	**free_env_array(char **env_array, int count)
{
	while (--count >= 0)
		free(env_array[count]);
	free(env_array);
	return (NULL);
}
