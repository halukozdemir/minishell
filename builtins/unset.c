/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:34:10 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 17:34:10 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	delete_env_node(t_env **env, t_env *node)
{
	if (node->next)
		node->next->prev = node->prev;
	if (node->prev)
		node->prev->next = node->next;
	if (node == *env)
		*env = (*env)->next;
	free(node->key);
	free(node->value);
	free(node);
}

void	unset(t_env **env, char **args)
{
	t_env		*temp;
	size_t		len_key;
	int			i;

	i = 1;
	while (args[i])
	{
		temp = *env;
		len_key = ft_strlen(args[i]);
		while (temp)
		{
			if (!ft_strncmp(temp->key, args[i], len_key)
				&& ft_strlen(temp->key) == len_key)
			{
				delete_env_node(env, temp);
				break ;
			}
			temp = temp->next;
		}
		i++;
	}
	g_exit_status = 0;
}
