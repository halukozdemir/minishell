/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 19:35:10 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/03 13:57:21 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_quote_states(char c, bool *in_single_quotes,
		bool *in_double_quotes)
{
	check_quotes(c, in_single_quotes, in_double_quotes);
}

char	*extract_key(char *input, int *i)
{
	int		j;
	char	*key;

	j = *i;
	while (ft_isalnum(input[j]) || input[j] == '_')
		j++;
	key = ft_substr(input, *i, j - *i);
	*i = j;
	return (key);
}

int	calculate_key_length(char *input, int *i, t_env *env)
{
	char	*key;
	char	*value;
	int		key_length;

	key = extract_key(input, i);
	value = get_env_value(env, key);
	if (value)
		key_length = ft_strlen(value);
	else
		key_length = 1;
	free(key);
	return (key_length);
}

int	update_total_length(char *input, t_env *env)
{
	int		i;
	int		total_len;
	bool	in_single_quotes;
	bool	in_double_quotes;

	i = 0;
	total_len = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	while (input[i])
	{
		update_quote_states(input[i], &in_single_quotes, &in_double_quotes);
		if (input[i] == '$' && !in_single_quotes)
		{
			i++;
			total_len += calculate_key_length(input, &i, env);
		}
		else
			total_len++;
		i++;
	}
	return (total_len);
}

int	handle_dollar_replacement(char *input, char *new_input, t_env *env,
		int *index)
{
	char	*key;
	char	*value;
	int		value_len;

	value_len = 0;
	key = extract_key(input, index);
	value = get_env_value(env, key);
	if (value)
	{
		ft_strlcpy(new_input, value, ft_strlen(value) + 1);
		value_len = ft_strlen(value);
	}
	free(key);
	return (value_len);
}
