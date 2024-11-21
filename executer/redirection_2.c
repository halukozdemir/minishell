/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:39:14 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/21 13:48:56 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_fd_2(t_jobs *jobs, int *indexes, int *fd)
{
	if (*fd == -1)
	{
		jobs->mshell->doll_quest = 1;
		return (dup2(jobs->mshell->backup_fd[0], 0),
			close(jobs->mshell->backup_fd[0]),
			dup2(jobs->mshell->backup_fd[1], 1),
			close(jobs->mshell->backup_fd[1]), -1);
	}
	if (!indexes[3])
		dup2(*fd, 0);
	else if (indexes[3] == 1)
		dup2(*fd, 1);
	if (*fd != 1)
		close(*fd);
	return (0);
}
