/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:17:14 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 16:17:45 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_job(t_job *job)
{
	if (!job)
		return ;
	if (job->args)
		free_str_arr(job->args);
	if (job->redir)
		free_redir(job->redir);
	free(job);
}

void	free_jobs_list(t_job *job_list)
{
	t_job	*temp;

	while (job_list)
	{
		temp = job_list->next_job;
		free_job(job_list);
		job_list = temp;
	}
}
