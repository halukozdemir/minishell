/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:20:39 by halozdem          #+#    #+#             */
/*   Updated: 2024/10/30 18:05:45 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_quotes(char c, bool *sq, bool *dq)
{
	if (c == '\'' && !*dq)
		*sq = !*sq;
	if (c == '"' && !*sq)
		*dq = !*dq;
}

void	replace_dollar_value(char **input, char *value, int start, int end)
{
	char	*new_input;
	int		new_len;

	if (!value)
		return;

	new_len = ft_strlen(*input) + ft_strlen(value) - (end - start);
	new_input = (char *)malloc(new_len + 1);
	if (!new_input)
	{
		perror("malloc failed");
		return ;
	}
	ft_strlcpy(new_input, *input, start + 1);
	ft_strlcat(new_input, value, new_len + 1);
	ft_strlcat(new_input, *input + end, new_len + 1);
	free(*input);
	*input = new_input;
}

void	remove_dollar(char **input, int start, int end)
{
	char	*new_input;
	int		new_len;

	new_len = ft_strlen(*input) - (end - start);
	new_input = (char *)malloc(new_len + 1);
	if (!new_input)
	{
		perror("malloc failed");
		return ;
	}
	ft_strlcpy(new_input, *input, start + 1);
	ft_strlcat(new_input, *input + end, new_len + 1);
	free(*input);
	*input = new_input;
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

void	get_dollar(char **input_ptr, t_env *env)
{
	t_dollar_params	params;
	int				len;

	len = calculate_new_length(*input_ptr, env);
	init_dollar_params(&params, *input_ptr, len);
	if (!params.new_input)
		return ;
	while (params.input[params.i])
	{
		check_quotes(params.input[params.i],
			&params.quotes.in_single_quotes, &params.quotes.in_double_quotes);
		if (params.input[params.i] == '$' && !params.quotes.in_single_quotes)
		{
			process_key(input_ptr, env, &params.i,
				params.quotes.in_single_quotes);
		}
		else
			params.new_input[params.idx++] = params.input[params.i++];
	}
	params.new_input[params.idx] = '\0';
	free(*input_ptr);
	*input_ptr = params.new_input;
}
