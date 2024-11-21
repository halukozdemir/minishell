/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:21:43 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 17:21:43 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler_sigint(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handler_heredoc(int status)
{
	if (status == SIGINT)
	{
		// rl_replace_line("", 0);
		ft_putchar_fd('\n', STDOUT_FILENO);
		// rl_on_new_line();
		exit(130);
	}
}

void	set_signal(int c)
{
	if (c == MAIN_SF)
	{
		signal(SIGINT, &handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (c == CHILD_SF)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (c == HDOC_SF)
	{
		signal(SIGINT, &handler_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
	else
		signal(SIGINT, SIG_IGN);
}
