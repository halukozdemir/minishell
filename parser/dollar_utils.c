/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:45:00 by haluk             #+#    #+#             */
/*   Updated: 2024/10/30 18:06:26 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	contains_special_operators(char *key)
{
	return (ft_strnstr(key, "<", ft_strlen(key)) != NULL
		|| ft_strnstr(key, ">", ft_strlen(key)) != NULL
		|| ft_strnstr(key, ">>", ft_strlen(key)) != NULL
		|| ft_strnstr(key, "<<", ft_strlen(key)) != NULL);
}

void	process_key(char **input_ptr, t_env *env, int *i, bool in_single_quotes)
{
	int		j;
	char	*key;
	char	*value;

	if (in_single_quotes)
	{
		(*i)++;
		return ;
	}
	j = *i + 1;
	while (ft_isalnum((*input_ptr)[j]) || (*input_ptr)[j] == '_')
		j++;
	key = ft_substr(*input_ptr, *i + 1, j - *i - 1);
	value = get_env_value(env, key);
	if (!value && ft_strlen(key) > 0)
	{
		remove_dollar(input_ptr, *i, j);
		*i = *i - 1;
	}
	else if (value)
	{
		replace_dollar_value(input_ptr, value, *i, j);
		*i = *i + ft_strlen(value) - 1;
	}
	free(key);
	(*i)++;
}

void	init_dollar_params(t_dollar_params *params, char *input, int len)
{
	params->input = input;
	params->new_input = (char *)malloc(len + 1);
	params->len = len;
	params->idx = 0;
	params->i = 0;
	params->quotes.in_single_quotes = false;
	params->quotes.in_double_quotes = false;
}
