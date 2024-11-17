/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:37:42 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 00:28:13 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	calculate_result(const char *str, int *index, long long *result)
{
	while (str[*index] && ft_isdigit(str[*index]))
	{
		if (*result > (LLONG_MAX - (str[*index] - '0')) / 10)
			return (EXIT_FAILURE);
		*result = *result * 10 + (str[*index] - '0');
		(*index)++;
	}
	return (EXIT_SUCCESS);
}

int	get_exit_value(char *str)
{
	long long	result;
	int			i;
	int			sign;

	result = 0;
	i = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	if (calculate_result(str, &i, &result) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	result = (result * sign) % 256;
	if (result < 0)
		result += 256;
	return (result);
}

void	handle_exit_argument(char **args, char *stripped_arg)
{
	int	exit_status;
	if (is_all_digit(stripped_arg) == EXIT_FAILURE)
	{
		exit_error(args[1], "numeric argument required");
		free(stripped_arg);
		exit(g_exit_status);
	}
	if (args[2])
	{
		g_exit_status = 1;
		write(2, "minishell: exit: too many arguments\n", 36);
		free(stripped_arg);
		return ;
	}
	exit_status = get_exit_value(stripped_arg);
	// if (exit_status == EXIT_FAILURE)
	// {
	// 	exit_error(args[1], "numeric argument required");
	// 	free(stripped_arg);
	// 	exit(g_exit_status);
	// }
	g_exit_status = exit_status;
	free(stripped_arg);
}
