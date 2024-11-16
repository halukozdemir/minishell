/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:18:55 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 17:19:17 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	display_env_vars(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("declare -x %s=\"\"\n", tmp->key);
		tmp = tmp->next;
	}
}

int	add_new_env_var(t_env **env, char *key, char *value)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (EXIT_FAILURE);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = ft_strdup("");
	new->prev = NULL;
	new->next = NULL;
	lstadd_back2(env, new);
	return (EXIT_SUCCESS);
}

char	*extract_key(char *arg)
{
	int		j;

	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	return (ft_substr(arg, 0, j));
}

char	*extract_value(char *arg)
{
	int		j;

	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	return (ft_substr(arg, j + 1, ft_strlen(arg) - j));
}
