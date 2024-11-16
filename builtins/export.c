/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:13:23 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/16 17:13:23 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	update_env_value(t_env *env, char *key, char *value)
{
	t_env		*temp;
	size_t		len_key;

	len_key = ft_strlen(key);
	temp = env;
	while (temp)
	{
		if (!ft_strncmp(temp->key, key, ft_strlen(temp->key))
			&& len_key == ft_strlen(temp->key))
		{
			free(temp->value);
			temp->value = ft_strdup(value);
			if (!temp->value)
				return (-1);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

static int	is_valid_identifier(char *str, char state)
{
	int		i;
	int		has_alpha;

	if (!str)
		return (EXIT_SUCCESS);
	i = 0;
	has_alpha = 0;
	while (str[i])
	{
		if (state && str[i] == '=')
			i++;
		if (ft_isalpha(str[i]))
			has_alpha = 1;
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (EXIT_FAILURE);
		i++;
	}
	if (has_alpha == 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	handle_export_error(char *args, const char *message)
{
	g_exit_status = 1;
	write(2, "minishell: export: `", 21);
	write(2, args, ft_strlen(args));
	write(2, "': ", 3);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	return (EXIT_FAILURE);
}

static int	process_argument(t_env *env, char *arg)
{
	char	*key;
	char	*value;
	int		state;

	key = extract_key(arg);
	if (is_valid_identifier(key, 0))
		return (handle_export_error(arg, "not a valid identifier"));
	value = extract_value(arg);
	state = update_env_value(env, key, value);
	if (state == -1
		|| (!state && add_new_env_var(&env, key, value) == EXIT_FAILURE))
	{
		free(key);
		free(value);
		return (EXIT_FAILURE);
	}
	free(key);
	free(value);
	return (EXIT_SUCCESS);
}

char	export(t_env *env, char **args)
{
	int	i;

	if (!args[1])
	{
		display_env_vars(env);
		g_exit_status = 0;
		return (EXIT_SUCCESS);
	}
	i = 1;
	while (args[i])
	{
		if (process_argument(env, args[i]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
