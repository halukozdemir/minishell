/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 19:35:40 by halozdem          #+#    #+#             */
/*   Updated: 2024/10/31 19:35:47 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	construct_new_input(char *input, char *new_input, t_env *env)
{
	int		i;
	int		len;
	bool	in_single_quotes;
	bool	in_double_quotes;

	i = 0;
	len = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	while (input[i])
	{
		update_quote_states(input[i], &in_single_quotes, &in_double_quotes);
		if (input[i] == '$' && !in_single_quotes)
		{
			i++;
			len += handle_dollar_replacement(input, new_input + len, env, &i);
		}
		else
		{
			new_input[len++] = input[i++];
		}
	}
	new_input[len] = '\0';
}

void	get_dollar(char **input_ptr, t_env *env)
{
	char	*input;
	int		total_len;
	char	*new_input;

	input = *input_ptr;
	total_len = update_total_length(input, env);
	new_input = (char *)malloc(total_len + 1);
	if (!new_input)
		return ;
	construct_new_input(input, new_input, env);
	free(*input_ptr);
	*input_ptr = new_input;
}
