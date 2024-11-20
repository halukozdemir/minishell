/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:50:45 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/20 17:01:57 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_new_job(t_job **temp, char **tokens)
{
	(*temp)->next_job = ft_calloc(1, sizeof(t_job));
	if (!(*temp)->next_job)
		return (free_str_arr(tokens), EXIT_FAILURE);
	*temp = (*temp)->next_job;
	(*temp)->redir = ft_calloc(1, sizeof(t_redir));
	if (!(*temp)->redir)
		return (free_str_arr(tokens), EXIT_FAILURE);
	(*temp)->redir->append_file = -1;
	(*temp)->redir->in_file = -1;
	(*temp)->redir->out_file = -1;
	(*temp)->redir->last_in = NONE_BOOL;
	(*temp)->redir->last_out = NONE_BOOL;
	return (EXIT_SUCCESS);
}

char	fill_jobs_from_tokens(t_mshell *shell, char **tokens)
{
	t_job	*temp;
	char	signal;
	int		i;

	signal = -1;
	shell->jobs->len = 1;
	temp = shell->jobs->job_list;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '|')
		{
			shell->jobs->len += 1;
			if (init_new_job(&temp, tokens) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		else if (handle_distribution(temp, tokens[i], &signal))
			return (free_str_arr(tokens), EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	reset_job_list(t_mshell *mshell)
{
	if (mshell->jobs->job_list)
		free_nec(mshell);
	mshell->jobs->job_list = ft_calloc(1, sizeof(t_job));
	mshell->jobs->job_list->redir = ft_calloc(1, sizeof(t_redir));
}

void	free_nec(t_mshell *mshell)
{
	t_job	*temp1;
	t_job	*next;

	temp1 = mshell->jobs->job_list;
	while (temp1)
	{
		next = temp1->next_job;
		free_job(temp1);
		temp1 = next;
	}
	mshell->jobs->job_list = NULL;
}

void	assign_signal(char *arg, char *signal)
{
	if (arg[0] == '<' && !arg[1])
		*signal = 0;
	else if (arg[0] == '>' && !arg[1])
		*signal = 1;
	else if (arg[0] == '<' && arg[1] == arg[0] && !arg[2])
		*signal = 2;
	else if (arg[0] == '>' && arg[1] == arg[0] && !arg[2])
		*signal = 3;
	else
		*signal = -1;
}
