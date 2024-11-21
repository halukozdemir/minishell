/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:44:17 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 18:44:17 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_pointer(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	handle_distribution(t_job *temp, char *arg, char *signal)
{
	char	state;
	char	**a;

	a = temp->args;
	if (!*arg)
		return (EXIT_FAILURE);
	state = ctrl_redir(temp, arg, signal);
	if (arg[0] == '<' && !arg[1])
		return (EXIT_SUCCESS);
	else if (arg[0] == '>' && !arg[1])
		return (EXIT_SUCCESS);
	else if (arg[0] == '<' && arg[1] == arg[0] && !arg[2])
		return (EXIT_SUCCESS);
	else if (arg[0] == '>' && arg[1] == arg[0] && !arg[2])
		return (EXIT_SUCCESS);
	if (state == EXIT_FAILURE)
		return (EXIT_FAILURE);
	else if (*signal == -1 && !ctrl_append(temp->redir, arg))
	{
		temp->args = str_arr_realloc(a, arg);
		if (!temp->args)
			return (free_double_pointer(temp->args), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = temp;
	}
}

int	process_user_input(char *input, t_mshell *mshell)
{
	char	**cmd;

	add_history(input);
	get_dollar(&input, mshell->jobs);
	if (!input[0] || check_unclosed_quotes(input, mshell))
		return ((free(input)), EXIT_FAILURE);
	cmd = get_token(input);
	if (!cmd || check_syntax_errors(cmd, mshell))
		return ((free(input)), (free_str_arr(cmd)), EXIT_FAILURE);
	if (fill_jobs_from_tokens(mshell, cmd) == EXIT_FAILURE)
		return ((free(input)), (free_str_arr(cmd)), EXIT_FAILURE);
	return ((free(input)), (free_str_arr(cmd)), EXIT_SUCCESS);
}

int	main(int argc, char **argv, char **env)
{
	t_mshell	mshell;
	t_env		*env_list;

	(void)argv;
	if (argc != 1)
		return (1);
	initialize_shell(&mshell, env, &env_list);
	run_shell(&mshell);
	cleanup_shell(&mshell);
	return (0);
}
