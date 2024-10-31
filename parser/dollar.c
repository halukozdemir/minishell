/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 19:05:04 by halozdem          #+#    #+#             */
/*   Updated: 2024/10/31 19:36:40 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_quotes(char c, bool *sq, bool *dq)
{
	if (c == '\'' && !*dq)
		*sq = !*sq;
	if (c == '\"' && !*sq)
		*dq = !*dq;
}

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0
			&& ft_strlen(env->key) == ft_strlen(key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

bool	contains_special_operators(char *key)
{
	if (ft_strnstr(key, "<", ft_strlen(key)) || ft_strnstr(key, ">",
			ft_strlen(key)) || ft_strnstr(key, ">>", ft_strlen(key))
		|| ft_strnstr(key, "<<", ft_strlen(key)))
		return (true);
	return (false);
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
	j = *i;
	while (ft_isalnum((*input_ptr)[j]) || (*input_ptr)[j] == '_')
		j++;
	key = ft_substr(*input_ptr, *i, j - *i);
	value = get_env_value(env, key);
	if (!value && ft_strlen(key) > 0)
	{
		remove_dollar(input_ptr, NULL, *i - 1, j);
		*i = *i - 1;
	}
	else if (value)
	{
		remove_dollar(input_ptr, value, *i - 1, j);
		*i = *i + ft_strlen(value) - 1;
	}
	free(key);
}

void	remove_dollar(char **input, char *value, int start, int end)
{
	char	*new_input;
	int		new_len;

	if (value)
		new_len = ft_strlen(*input) + ft_strlen(value) - (end - start);
	else
		new_len = ft_strlen(*input) - (end - start);
	new_input = (char *)malloc(new_len + 1);
	if (!new_input)
		return ;
	ft_strlcpy(new_input, *input, start + 1);
	if (value)
		ft_strlcat(new_input, value, new_len + 1);
	ft_strlcat(new_input, *input + end, new_len + 1);
	free(*input);
	*input = new_input;
}
