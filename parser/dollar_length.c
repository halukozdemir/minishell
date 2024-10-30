/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_length.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:45:00 by haluk             #+#    #+#             */
/*   Updated: 2024/10/30 18:06:43 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	calculate_value_length(char *input, t_env *env, int *i, t_quotes *quotes)
{
	int		j;
	char	*key;
	char	*value;
	int		length;

	length = 0;
	(*i)++; // $ işaretinden sonra ilerliyoruz
	j = *i;
	while (ft_isalnum(input[j]) || input[j] == '_')
		j++;
	key = ft_substr(input, *i, j - *i);
	value = get_env_value(env, key);
	if (value)
		length = ft_strlen(value);
	free(key);
	*i = j; // `i`'yi `$` değişkenini tamamen geçecek şekilde güncelliyoruz
	return (length);
}

int	calculate_new_length(char *input, t_env *env)
{
	int			i;
	int			total_len;
	t_quotes	quotes;

	i = 0;
	total_len = 0;
	quotes.in_single_quotes = false;
	quotes.in_double_quotes = false;
	while (input[i])
	{
		check_quotes(input[i],
			&quotes.in_single_quotes, &quotes.in_double_quotes);
		if (input[i] == '$' && !quotes.in_single_quotes)
			total_len += calculate_value_length(input, env, &i, &quotes);
		else
		{
			total_len++;
			i++;
		}
	}
	return (total_len);
}
