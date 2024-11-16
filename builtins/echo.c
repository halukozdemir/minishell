/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:58:39 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 16:58:39 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_argument(char *arg, bool *in_quote, char *quote_type)
{
	int		j;

	j = 0;
	while (arg[j])
	{
		if (ft_strchr(QUOTES, arg[j]))
		{
			if (!(*in_quote))
			{
				*quote_type = arg[j];
				*in_quote = true;
			}
			else if (arg[j] == *quote_type)
				*in_quote = false;
			else if (*in_quote)
				write(1, &arg[j], 1);
		}
		else
			write(1, &arg[j], 1);
		j++;
	}
}

void	echo(t_job *job)
{
	int		i;
	int		newline;
	bool	in_quote;
	char	quote_type;

	newline = 1;
	in_quote = false;
	i = 1;
	if (job->args[1] && ft_strncmp(job->args[1], "-n", 2) == 0
		&& ft_strlen(job->args[1]) == 2)
	{
		newline = 0;
		i++;
	}
	while (job->args[i])
	{
		print_argument(job->args[i], &in_quote, &quote_type);
		if (job->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
}
