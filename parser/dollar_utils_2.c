/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:07:44 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/21 13:56:05 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_variable_length(char *input, int *i, t_jobs *jobs)
{
	int		start;
	char	*key;
	char	*value;
	int		len;

	start = *i;
	key = NULL;
	value = NULL;
	len = 0;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	key = ft_substr(input, start, *i - start);
	if (!key)
		return (0);
	value = get_env_value(jobs->env, key);
	if (value)
		len += ft_strlen(value);
	free(key);
	return (len);
}

void	check_quotes(char c, bool *sq, bool *dq)
{
	if (c == '\'' && !(*dq))
		*sq = !(*sq);
	if (c == '\"' && !(*sq))
		*dq = !(*dq);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (!ft_strncmp(temp->key, key, ft_strlen(key))
			&& ft_strlen(temp->key) == ft_strlen(key))
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

int	handle_exit_status(t_jobs *jobs, char *new_input, int *len)
{
	char	*value;

	value = ft_itoa(jobs->mshell->doll_quest);
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
