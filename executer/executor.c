/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:25:40 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 16:25:40 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	no_pipe(t_jobs *jobs, t_job *job)
{
	int	fd;

	is_builtin(job);
	fd = get_fd(jobs, job, -1);
	if (fd == -1)
		return (EXIT_FAILURE);
	if (job->is_builtin == false)
	{
		job->pid = fork();
		if (job->pid == 0)
		{
			set_signal(CHILD_SF);
			run_cmd(jobs, job);
			exit(jobs->mshell->doll_quest);
		}
	}
	else
		return (ctrl_builtins(jobs, job));
	return (EXIT_SUCCESS);
}

void	wait_child(t_mshell *mshell)
{
	t_job	*temp_job;
	int		temp_status;
	int		i;

	temp_job = mshell->jobs->job_list;
	if (mshell->jobs->len == 1 && temp_job->is_builtin == true)
		return ;
	while (temp_job)
	{
		signal(SIGINT, &handler_sigint);
		i = waitpid(temp_job->pid, &temp_status, 0);
		if (i < 0)
			continue ;
		is_builtin(temp_job);
		if (mshell->jobs->len == 1 && temp_job->is_builtin == true)
			break ;
		if (WIFEXITED(temp_status))
			mshell->doll_quest = WEXITSTATUS(temp_status);
		else if (WIFSIGNALED(temp_status))
			mshell->doll_quest = 128 + WTERMSIG(temp_status);
		temp_job = temp_job->next_job;
	}
}

int	handle_job(t_mshell *mshell, t_job *temp_job)
{
	if (mshell->jobs->len == 1)
	{
		if (temp_job->redir->eof && heredoc(mshell->jobs, temp_job, 1))
			return (0);
		if (no_pipe(mshell->jobs, temp_job))
			return (EXIT_SUCCESS);
	}
	else
	{
		if (temp_job->redir->eof && heredoc(mshell->jobs, temp_job, 0))
			return (0);
		if (mshell->doll_quest == 130)
			return (0);
		if (pipe_handle(mshell->jobs, temp_job))
			return (EXIT_SUCCESS);
		mshell->doll_quest = 0;
	}
	return (1);
}

char	executor(t_mshell *mshell)
{
	t_job	*temp_job;
	int		state;

	state = 1;
	mshell->backup_fd[0] = dup(STDIN_FILENO);
	mshell->backup_fd[1] = dup(STDOUT_FILENO);
	temp_job = mshell->jobs->job_list;
	while (temp_job)
	{
		state = handle_job(mshell, temp_job);
		if (state != 1)
			break ;
		temp_job = temp_job->next_job;
	}
	if (state)
	{
		dup2(mshell->backup_fd[0], 0);
		close(mshell->backup_fd[0]);
		dup2(mshell->backup_fd[1], 1);
		close(mshell->backup_fd[1]);
		wait_child(mshell);
	}
	return (EXIT_SUCCESS);
}
