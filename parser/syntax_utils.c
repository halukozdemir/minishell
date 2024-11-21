/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:54:57 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/21 17:08:07 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_special_char(const char *token)
{
	return (ft_strncmp(token, "|", 2) == 0 || ft_strncmp(token, "<", 2) == 0
		|| ft_strncmp(token, ">", 2) == 0 || ft_strncmp(token, "<<", 3) == 0
		|| ft_strncmp(token, ">>", 3) == 0);
}

static bool	syntax_error_loop(char **tokens, int i, t_mshell *mshell)
{
	if (ft_strncmp(tokens[i], "|", 2) == 0)
	{
		if (i == 0 || tokens[i + 1] == NULL || is_special_char(tokens[i
					+ 1]))
		{
			ft_putendl_fd("Syntax error near unexpected token '|'", 2);
			return (mshell->doll_quest = 258, true);
		}
	}
	if (is_special_char(tokens[i]) && ft_strncmp(tokens[i], "|", 2) != 0)
	{
		if (tokens[i + 1] == NULL || is_special_char(tokens[i + 1]))
		{
			ft_putendl_fd("Syntax error near unexpected token", 2);
			return (mshell->doll_quest = 258, true);
		}
	}
	if (ft_strncmp(tokens[i], "||", 3) == 0)
	{
		ft_putendl_fd("Syntax error near unexpected token", 2);
		return (mshell->doll_quest = 258, true);
	}
	return (false);
}

bool	check_syntax_errors(char **tokens, t_mshell *mshell)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (syntax_error_loop(tokens, i, mshell) == true)
			return (true);
		i++;
	}
	return (false);
}

bool	check_unclosed_quotes(const char *input, t_mshell *mshell)
{
	char	quote;

	quote = '\0';
	while (*input)
	{
		if ((ft_strchr(QUOTES, *input)) && (quote == '\0' || quote == *input))
		{
			if (quote == '\0')
				quote = *input;
			else
				quote = '\0';
		}
		input++;
	}
	if (quote != '\0')
	{
		ft_putendl_fd("Syntax error: Unclosed quote", 2);
		return (mshell->doll_quest = 258, true);
	}
	return (false);
}
