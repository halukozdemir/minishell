/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 13:38:02 by halozdem          #+#    #+#             */
/*   Updated: 2024/10/14 17:57:27 by halozdem         ###   ########.fr       */
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

typedef struct s_redir
{
	//int		type;
	char	*file;
	int		fd;
}  t_redir;

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

//yeni structlarım

typedef	struct s_args
{
	char	*str;
}	t_args;

typedef struct s_command
{
	char			*str;
	t_args			*args;
	struct s_list	*next;
}	t_command;

void	init_struct(t_command *cmd);
t_env	*new_node(t_env **lst, char *env, int end, int i);
t_env	**envfunc(char **env, int n);
char	**get_token(char *input);
char	**split_words(char *input, char **str, unsigned int word_count);
char	*put_word(char *word, char *input, int i, int word_len);
int		word_counter(char *input);
char	**split_by_meta(char **str);
void	run_cmds(char *input, char **env);
int		syntax_cont(char *input, bool *has_error);
int	pipe_cont(char *input);
int	redir_cont(char *input);
int	redir_cont2(char *input, int i, char c);
int	quote_cont(char *input);
int	is_meta(char *input, int i);

#endif
