/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 13:38:02 by halozdem          #+#    #+#             */
/*   Updated: 2024/10/21 01:47:47 by halozdem         ###   ########.fr       */
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
#include <fcntl.h>  // Dosya modları ve open() için gerekli

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
char	**get_token(char *input);
char	**split_words(char *input, char **str, unsigned int word_count);
char	*put_word(char *word, char *input, int i, int word_len);
int		word_counter(char *input);
char	**split_by_meta(char **str);
void	run_cmds(char *input, t_env *env);
int		syntax_cont(char *input, bool *has_error);
int		pipe_cont(char *input);
int		redir_cont(char *input);
int		redir_cont2(char *input, int i, char c);
int		quote_cont(char *input);
int		is_meta(char *input, int i);
int		is_space(char c);
void	get_dollar(char **input, t_env *env);
t_env	**lstadd_back2(t_env **lst, t_env *new);
t_env	*envfunc2(char	**env);
char	*funcval(char	*env, int start);
char	*funckey(char	*env, int end);
void	run_env(t_env *env2);
void	check_quotes(char c, bool *sq, bool *dq);
void	replace_dollar_with_value_or_remove(char **input, char *key,
			char *value, int start, int end);
char	*get_env_value(t_env *env, char *key);
void	process_key(char **input_ptr, t_env *env, int *i);
void	get_dollar(char **input_ptr, t_env *env);
void handle_redirection(char *filename, int redir_type);
void execute_pipe(char **cmd1, char **cmd2, char **env);
void	execute_command(char **cmd, char **env);
void	executor(char **cmds, char **env);
int	contains_pipe(char **cmds);
int	contains_redirection(char **cmds);


#endif
