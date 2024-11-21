/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:52:24 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/21 21:19:12 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize_shell(t_mshell *mshell, char **env, t_env **env_list)
{
	mshell->doll_quest = 0;
	mshell->is_exit = 0;
	*env_list = envfunc2(env);
	mshell->jobs = ft_calloc(1, sizeof(t_jobs));
	mshell->jobs->mshell = mshell;
	mshell->jobs->env = *env_list;
}

void	run_shell(t_mshell *mshell)
{
	char	*input;
	char	*input2;

	while (1)
	{
		reset_job_list(mshell);
		set_signal(MAIN_SF);
		input = readline("minishell> ");
		input2 = ft_strtrim(input, " \t\r\v\f");
		free(input);
		if (!input2)
		{
			free(input2);
			break ;
		}
		set_signal(3);
		if (process_user_input(input2, mshell) == EXIT_FAILURE)
		{
			free(input2);
			continue ;
		}
		executor(mshell);
	}
}

void	cleanup_shell(t_mshell *mshell)
{
	free_jobs_list(mshell->jobs->job_list);
	free_env_list(mshell->jobs->env);
	free(mshell->jobs);
}
