/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:48:19 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 18:57:25 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	append_files_order(t_job *temp, char *arg)
{
	temp->redir->files_order = str_arr_realloc(temp->redir->files_order, arg);
	if (!temp->redir->files_order)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	ctrl_redir(t_job *temp, char *arg, char *signal)
{
	char	*arg_net;

	arg_net = NULL;
	if (*signal == 0)
	{
		*signal = -1;
		temp->redir->last_in = IN;
		arg_net = ft_strtrim(arg, "\"");
		temp->redir->in_files = str_arr_realloc(temp->redir->in_files,
				arg_net);
		if (!temp->redir->in_files || append_files_order(temp, arg_net))
			return (free(arg_net), EXIT_FAILURE);
	}
	else if (*signal == 1)
	{
		*signal = -1;
		temp->redir->last_out = OUT;
		arg_net = ft_strtrim(arg, "\"");
		temp->redir->out_files = str_arr_realloc(temp->redir->out_files,
				arg_net);
		if (!temp->redir->out_files || append_files_order(temp, arg_net))
			return (free(arg_net), EXIT_FAILURE);
	}
	return (ctrl_redir_continue(temp, arg, signal, arg_net));
}

char	ctrl_redir_continue(t_job *temp, char *arg, char *signal,
		char *arg_net)
{
	if (*signal == 2)
	{
		*signal = -1;
		temp->redir->last_in = HDOC;
		arg_net = ft_strtrim(arg, "\"");
		temp->redir->eof = str_arr_realloc(temp->redir->eof, arg_net);
		if (!temp->redir->eof || append_files_order(temp, arg_net))
			return (free(arg_net), EXIT_FAILURE);
	}
	else if (*signal == 3)
	{
		*signal = -1;
		temp->redir->last_out = APPEND;
		arg_net = ft_strtrim(arg, "\"");
		temp->redir->appends = str_arr_realloc(temp->redir->appends, arg_net);
		if (!temp->redir->appends || append_files_order(temp, arg_net))
			return (free(arg_net), EXIT_FAILURE);
	}
	else
		assign_signal(arg, signal);
	free(arg_net);
	return (EXIT_SUCCESS);
}

int	check_redir_list(char **redir_list, char *arg_trimmed)
{
	int	len_str;
	int	len_arg;
	int	len;

	if (!redir_list)
		return (0);
	len = str_arr_len(redir_list);
	len_arg = ft_strlen(arg_trimmed);
	len_str = ft_strlen(redir_list[len - 1]);
	if (!ft_strncmp(redir_list[len - 1], arg_trimmed, len_str)
		&& len_str == len_arg)
		return (1);
	return (0);
}

char	ctrl_append(t_redir *redir, char *arg)
{
	char	*arg_trimmed;

	arg_trimmed = ft_strtrim(arg, "\"");
	if (!arg_trimmed)
		return (1);
	if (check_redir_list(redir->appends, arg_trimmed)
		|| check_redir_list(redir->in_files, arg_trimmed)
		|| check_redir_list(redir->out_files, arg_trimmed)
		|| check_redir_list(redir->eof, arg_trimmed))
	{
		free(arg_trimmed);
		return (1);
	}
	free(arg_trimmed);
	return (0);
}
