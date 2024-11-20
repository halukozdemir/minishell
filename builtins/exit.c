/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:35:44 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 16:35:44 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>

char	*strip_quotes(char *str)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

char	exit_error(t_jobs *jobs, char *args, const char *message)
{
	write(2, "minishell: exit: ", 17);
	write(2, args, ft_strlen(args));
	write(2, ": ", 2);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	jobs->mshell->doll_quest = 255;
	return (EXIT_FAILURE);
}

char	is_all_digit(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (EXIT_FAILURE);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	exit_d(t_jobs *jobs, char **args)
{
	char	*stripped_arg;

	write(1, "exit\n", 5);
	if (args[1])
	{
		stripped_arg = strip_quotes(args[1]);
		if (!stripped_arg)
		{
			exit_error(jobs, args[1], "memory allocation error");
			exit(jobs->mshell->doll_quest);
		}
		handle_exit_argument(jobs, args, stripped_arg);
	}
	else
		jobs->mshell->doll_quest = 0;
	exit(jobs->mshell->doll_quest);
}
