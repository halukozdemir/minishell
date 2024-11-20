/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:31:26 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/20 15:35:13 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	access_error(char *file, const char *message)
{
	g_exit_status = 1;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd((char *)message, 2);
}

void	handle_execution_error(char *path)
{
	t_stat	stat_t;

	stat(path, &stat_t);
	if (S_ISDIR(stat_t.st_mode))
	{
		g_exit_status = 1;
		write(2, "minishell: ", 12);
		write(2, path, ft_strlen(path));
		write(2, ": Is a directory\n", 18);
		exit(126);
	}
	else if (!S_ISREG(stat_t.st_mode))
	{
		g_exit_status = 1;
		write(2, "minishell: ", 12);
		write(2, path, ft_strlen(path));
		write(2, ": No such file or directory\n", 29);
		exit(127);
	}
	if (access(path, X_OK))
	{
		g_exit_status = 1;
		ft_putendl_fd(":  Permission denied", 2);
		exit(126);
	}
}

void	handle_no_env_path(t_jobs *jobs, t_job *job)
{
	if (!access(job->args[0], X_OK))
	{
		execve(job->args[0], job->args, env_to_double_pointer(jobs->env));
		exit(127);
	}
	else
	{
		g_exit_status = 1;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(job->args[0], 2);
		ft_putendl_fd(": No such file or directory", 2);
	}
}

void	handle_exec_path_error(t_job *job)
{
	g_exit_status = 1;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(job->args[0], 2);
	ft_putendl_fd(": command not found", 2);
	exit(127);
}

void	print_error_message(char *file_i, char *message)
{
	write(2, "minishell: ", 12);
	write(2, file_i, ft_strlen(file_i));
	write(2, message, ft_strlen(message));
}
