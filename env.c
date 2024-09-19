/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beyildiz <beyildiz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 15:45:07 by beyildiz          #+#    #+#             */
/*   Updated: 2024/09/19 19:04:38 by beyildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lstadd_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	temp = *lst;
	if (!*lst)
		*lst = new;
	else
	{
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = new;
	}
	printf("%s=%s\n", new->key, new->value);
}

char	*funckey(char	*env, int end)
{
	char	*key;
	key = (char *)malloc(end + 1);
	key[end] = '\0';
	while (end >= 0)
	{
		key[end] = env[end];
		end--;
	}
	return (key);
}

char	*funcval(char	*env, int start)
{
	char	*val;
	int		end;
	int		i;

	i = 0;
	end = start;
	while (env[end])
	{
		end++;
	}
	end--;
	val = (char *)malloc(end - start+ 1);
	while (env[start])
	{
		val[i] = env[start];
		start++;
		i++;
	}
	val[start] = '\0';
	return (val);
}

int	envfunc(char	**env)
{
	int		i;
	int		end;
	t_env	*new;
	t_env	**lst;

	i = 0;
	while (env[i])
	{
		end = 0;
		while ((env[i][end] && env[i][end] != '='))
		{
			end++;
		}
		new = (t_env *)malloc(sizeof(t_env));
		lst = (t_env **)malloc(sizeof(t_env));
		if (!new)
			return (0);
		new->next = NULL;
		new->key = funckey(env[i], end - 1);
		new->value = funcval(env[i], end + 1);
		lstadd_back(lst, new);
		if (env[i])
			i++;
	}
	return (0);
}
