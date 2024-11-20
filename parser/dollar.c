/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/11/18 13:06:59 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 13:06:59 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	calculate_variable_length(char *input, int *i, t_jobs *jobs)
{
	int		len;
	char	*temp;

	len = 0;
	if (input[*i] == '?')
	{
		temp = ft_itoa(jobs->mshell->doll_quest);
		if (!temp)
			return (0);
		len += ft_strlen(temp);
		free(temp);
	}
	else
	{
		len += get_variable_length(input, i, jobs);
	}
	return (len);
}

int	calculate_length(char *input, t_jobs *jobs)
{
	int		len;
	int		i;
	bool	in_sq;
	bool	in_dq;

	len = 0;
	i = 0;
	in_sq = false;
	in_dq = false;
	while (input[i])
	{
		check_quotes(input[i], &in_sq, &in_dq);
		if (input[i] == '$' && !in_sq)
		{
			i++;
			len += calculate_variable_length(input, &i, jobs);
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

void	process_variable(char *input, char *new_input, t_jobs *jobs,
		int *indices)
{
	indices[0]++;
	if (input[indices[0]] == '\0')
		new_input[indices[1]++] = '$';
	else if (input[indices[0]] == '?')
	{
		handle_exit_status(new_input, &indices[1]);
		indices[0]++;
	}
	else if (ft_isalnum(input[indices[0]]) || input[indices[0]] == '_')
		expand_variable(input, new_input, jobs, indices);
	else
		new_input[indices[1]++] = '$';
}

void	process_input(char *input, char *new_input, t_jobs *jobs, int *indices)
{
	bool	in_sq;
	bool	in_dq;

	in_sq = false;
	in_dq = false;
	while (input[indices[0]])
	{
		check_quotes(input[indices[0]], &in_sq, &in_dq);
		if (input[indices[0]] == '$' && !in_sq)
			process_variable(input, new_input, jobs, indices);
		else
			new_input[indices[1]++] = input[indices[0]++];
	}
	new_input[indices[1]] = '\0';
}

void	get_dollar(char **input_ptr, t_jobs *jobs)
{
	char	*new_input;
	char	*input;
	int		indices[2];

	indices[0] = 0;
	indices[1] = 0;
	input = *input_ptr;
	new_input = (char *)malloc(calculate_length(input, jobs) + 1);
	if (!new_input)
		return ;
	process_input(input, new_input, jobs, indices);
	free(*input_ptr);
	*input_ptr = new_input;
}
