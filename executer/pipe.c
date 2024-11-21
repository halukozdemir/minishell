/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:49:46 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/21 13:46:31 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_child_process(t_jobs *jobs, t_job *job, int *pipe_fd)
{
	int	fd;

	set_signal(CHILD_SF);
	close(pipe_fd[0]);
	if (job->next_job)
	{
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
	}
	fd = 1;
	if (job->redir->in_files || job->redir->out_files || job->redir->appends)
		fd = get_fd(jobs, job, -1);
	if (fd == -1)
		exit(1);
	is_builtin(job);
	if (job->is_builtin == false)
		run_cmd(jobs, job);
	exit(ctrl_builtins(jobs, job));
}

char	pipe_handle(t_jobs *jobs, t_job *job)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(127);
	}
	job->pid = fork();
	if (job->pid == 0)
		pipe_child_process(jobs, job, pipe_fd);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (EXIT_SUCCESS);
}
