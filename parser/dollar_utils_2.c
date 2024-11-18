/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:07:44 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 13:50:21 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_quotes(char c, bool *sq, bool *dq)
{
	if (c == '\'' && !(*dq))
		*sq = !(*sq);
	if (c == '\"' && !(*sq))
		*dq = !(*dq);
}

void	replace_dollar(char **input, char *value, int start, int end)
{
	char	*new_input;
	int		new_len;

	new_len = ft_strlen(*input) - (end - start);
	if (value)
		new_len += ft_strlen(value);
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

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strncmp(env->key, key, ft_strlen(key))
			&& ft_strlen(env->key) == ft_strlen(key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	handle_exit_status(char *new_input, int *len)
{
	char	*value;

	value = ft_itoa(g_exit_status);
	if (!value)
		return (0);
	ft_strlcpy(new_input + *len, value, ft_strlen(value) + 1);
	*len += ft_strlen(value);
	free(value);
	return (1);
}

void	expand_variable(char *input, char *new_input, t_jobs *jobs,
		int *indices)
{
	char	*key;
	char	*value;
	int		start;

	start = indices[0];
	while (ft_isalnum(input[start]) || input[start] == '_')
		start++;
	key = ft_substr(input, indices[0], start - indices[0]);
	value = get_env_value(jobs->env, key);
	free(key);
	if (value)
	{
		ft_strlcpy(new_input + indices[1], value, ft_strlen(value) + 1);
		indices[1] += ft_strlen(value);
	}
	indices[0] = start;
}
