/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beyildiz <beyildiz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 13:38:02 by halozdem          #+#    #+#             */
/*   Updated: 2024/09/20 17:17:20 by beyildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "lib/libft/libft.h"
#include <stdbool.h>


typedef struct s_redirs
{
	//int		type;
	char	*file;
	int		fd;
}  t_redirs;

typedef struct s_arg
{
	char	*content;
	char	*next;
}	t_arg;

typedef struct s_execcmd
{
	int		out_file;
	int		in_file;
	t_arg	*args;
	t_arg	*redirs;
}	t_execcmd;

typedef	struct	s_cmd
{
	int		index;
	int		dquote_count;
	int		squote_count;
	bool	dquote;
	bool	squote;
	char	*line;
	struct	s_env	*t_env;
}	t_cmd;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
	struct s_env		*prev;
}	t_env;

void	init_struct(t_cmd *cmd);
t_env	*new_node(t_env **lst, char *env, int end, int i);
t_env	**envfunc(char **env, int n);
int		builtinfunc(char *input, char **env);

#endif
