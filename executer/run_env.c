/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:24:53 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 16:24:53 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lstadd_back2(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
}

char	*funckey(char *env, int end)
{
	char	*key;
	int		i;

	key = (char *)malloc(end + 2);
	if (!key)
		return (NULL);
	i = 0;
	while (i <= end)
	{
		key[i] = env[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

char	*funcval(char *env, int start)
{
	char	*val;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (env[start + len])
		len++;
	val = (char *)malloc(len + 1);
	if (!val)
		return (NULL);
	while (env[start])
	{
		val[i] = env[start];
		start++;
		i++;
	}
	val[i] = '\0';
	return (val);
}

t_env	*create_env_node(char *env_entry)
{
	int		end;
	t_env	*new;

	end = 0;
	while (env_entry[end] && env_entry[end] != '=')
		end++;
	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = funckey(env_entry, end - 1);
	if (!new->key)
		return (free(new), NULL);
	new->value = funcval(env_entry, end + 1);
	if (!new->value)
		return (free(new->key), free(new), NULL);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_env	*envfunc2(char **env)
{
	int		i;
	t_env	*new;
	t_env	*lst;

	lst = NULL;
	i = 0;
	while (env[i])
	{
		new = create_env_node(env[i]);
		if (!new)
			return (free_env_list(lst), NULL);
		lstadd_back2(&lst, new);
		i++;
	}
	return (lst);
}
