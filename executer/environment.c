/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:45:58 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 16:46:32 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_env_count(t_env *env_list)
{
	int		count;
	t_env	*temp;

	temp = env_list;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

char	**env_to_double_pointer(t_env *env_list)
{
	t_env	*temp;
	char	**env_array;
	char	*temp_str;
	int		count;
	int		i;

	count = get_env_count(env_list);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	temp = env_list;
	i = 0;
	while (i < count)
	{
		temp_str = ft_strjoin(temp->key, "=");
		if (!temp_str)
			return (free_env_array(env_array, i));
		env_array[i] = ft_strjoin(temp_str, temp->value);
		free(temp_str);
		if (!env_array[i])
			return (free_env_array(env_array, i));
		i++;
		temp = temp->next;
	}
	return ((env_array[count] = NULL), env_array);
}
