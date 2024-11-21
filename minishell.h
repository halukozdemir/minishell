/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:32:04 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/21 14:32:04 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SPECIAL_CHARS "<>| \0"
# define QUOTES "'\""

# define MAIN_SF 0
# define CHILD_SF 1
# define HDOC_SF 2

# include "lib/libft/libft.h"
# include "sys/stat.h"
# include "sys/wait.h"
# include "termcap.h"
# include "termios.h"
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_job	t_job;
typedef struct s_jobs	t_jobs;
typedef struct s_redir	t_redir;
typedef struct s_env	t_env;
typedef struct stat		t_stat;

typedef struct s_split
{
	int		i;
	int		count;
	int		word_len;
	bool	in_quote;
	bool	in_word;
	char	quote_type;
	int		start;
}	t_split;

typedef struct s_env
{
	char	*key;
	char	*value;
	t_env	*next;
	t_env	*prev;
}	t_env;

typedef enum e_type
{
	NONE_TYPE,
	PIPE,
	EXEC
}	t_type;

typedef enum e_bool
{
	NONE_BOOL,
	OUT,
	APPEND,
	IN,
	HDOC
}	t_bool;

struct s_redir
{
	int		in_file;
	int		out_file;
	int		append_file;
	t_bool	last_out;
	t_bool	last_in;
	char	**in_files;
	char	**out_files;
	char	**appends;
	char	**eof;
	char	**files_order;
};

struct s_job
{
	bool	is_builtin;
	pid_t	pid;
	char	**args;
	t_redir	*redir;
	t_job	*next_job;
};

typedef struct s_mshell	t_mshell;

struct s_jobs
{
	t_mshell	*mshell;
	t_type		type;
	t_job		*job_list;
	t_env		*env;
	int			len;
};

typedef struct termios	t_termios;

struct s_mshell
{
	t_jobs		*jobs;
	t_termios	termios;
	char		is_exit;
	int			doll_quest;
	char		**success_arr;
	int			backup_fd[2];
};

typedef struct s_counters
{
	int	i_out;
	int	i_app;
	int	i_in;
}	t_counters;

char	cd(t_jobs *jobs, char *path, t_env *env);
char	handle_pwd(t_jobs *jobs);
char	handle_cd(t_jobs *jobs, char *arg, t_env *env);
char	handle_echo(t_jobs *jobs, t_job *job);
char	handle_env(t_jobs *jobs);
char	handle_exit(t_jobs *jobs, char **args);
char	handle_unset(t_jobs *jobs, t_env **env, char **args);
char	handle_export(t_jobs *jobs, char **args);
void	is_builtin(t_job *job);
char	ctrl_builtins(t_jobs *jobs, t_job *job);
void	print_argument(char *arg, bool *in_quote, char *quote_type);
void	echo(t_job *job);
void	env(t_env *env);
int		calculate_result(const char *str, int *index, long long *result);
int		get_exit_value(char *str);
void	handle_exit_argument(t_jobs *jobs, char **args, char *stripped_arg);
char	*strip_quotes(char *str);
char	exit_error(t_jobs *jobs, char *args, const char *message);
char	is_all_digit(char *str);
void	exit_d(t_jobs *jobs, char **args);
void	display_env_vars(t_env *env);
int		add_new_env_var(t_env **env, char *key, char *value);
char	*extract_key(char *arg);
char	*extract_value(char *arg);
char	export(t_jobs *jobs, char **args);
char	pwd(void);
void	delete_env_node(t_env **env, t_env *node);
void	unset(t_jobs *jobs, t_env **env, char **args);
void	run_cmd(t_jobs *jobs, t_job *job);
int		get_env_count(t_env *env_list);
char	**env_to_double_pointer(t_env *env_list);
void	handle_stat_error(char *file_i);
char	redir_error(t_jobs *jobs, t_job *job, char *file_i, int fd);
char	file_control(t_jobs *jobs, t_job *job, char *file, int fd);
char	**free_env_array(char **env_array, int count);
void	access_error(t_jobs *jobs, char *file, const char *message);
void	handle_execution_error(t_jobs *jobs, char *path);
void	handle_no_env_path(t_jobs *jobs, t_job *job);
void	handle_exec_path_error(t_jobs *jobs, t_job *job);
void	print_error_message(char *file_i, char *message);
void	free_job(t_job *job);
void	free_jobs_list(t_job *job_list);
void	free_str_arr(char **arr);
int		str_arr_len(char **str_arr);
char	**str_arr_realloc(char **str_arr, char *element);
char	*find_path(char *path, char *cmd);
void	free_redir(t_redir *redir);
char	no_pipe(t_jobs *jobs, t_job *job);
void	wait_child(t_mshell *mshell);
int		handle_job(t_mshell *mshell, t_job *temp_job);
char	executor(t_mshell *mshell);
char	heredoc(t_jobs *jobs, t_job *job, char state);
void	child_process(t_jobs *jobs, t_job *job, char state, int *pipe_fd);
void	handle_eof_condition(t_job *job, int *i, char *buffer, int *pipe_fd);
void	wait_for_child(t_jobs *jobs, pid_t pid, int *temp_status, char state);
void	pipe_child_process(t_jobs *jobs, t_job *job, int *pipe_fd);
char	pipe_handle(t_jobs *jobs, t_job *job);
int		get_fd(t_jobs *jobs, t_job *job, int i);
int		get_fd_2(t_jobs *jobs, int *indexes, int *fd);
int		process_redirection(t_jobs *jobs, t_job *job, int *indexes, int i);
int		open_out_file(t_jobs *jobs, t_job *job, char *file, int indexes[4]);
int		open_append_file(t_jobs *jobs, t_job *job, char *file, int indexes[4]);
int		open_in_file(t_jobs *jobs, t_job *job, char *file, int indexes[4]);
void	lstadd_back2(t_env **lst, t_env *new);
char	*funckey(char *env, int end);
char	*funcval(char *env, int start);
t_env	*create_env_node(char *env_entry);
t_env	*envfunc2(char **env);
void	handler_sigint(int sig);
void	handler(int sig);
void	handler_heredoc(int status);
void	set_signal(int c);
void	check_quotes(char c, bool *sq, bool *dq);
char	*get_env_value(t_env *env, char *key);
int		handle_exit_status(t_jobs *jobs, char *new_input, int *len);
int		get_variable_length(char *input, int *i, t_jobs *jobs);
void	expand_variable(char *input, char *new_input, t_jobs *jobs,
			int *indices);
int		calculate_variable_length(char *input, int *i, t_jobs *jobs);
int		calculate_length(char *input, t_jobs *jobs);
void	process_variable(char *input, char *new_input, t_jobs *jobs,
			int *indices);
void	process_input(char *input, char *new_input, t_jobs *jobs,
			int *indices);
void	get_dollar(char **input_ptr, t_jobs *jobs);
void	handle_quotes(t_split *s, char *input);
void	handle_special_chars(t_split *s, char *input);
void	process_quotes_and_length(t_split *s, char *input);
void	handle_word_allocation(char *input, char **str, t_split *s, int word);
char	**free_array(char **ptr, int i);
void	init_split(t_split *s);
int		skip_whitespace(char *input, int i, t_split *s);
char	*put_word(char *word, char *input, int start, int word_len);
char	**split_words(char *input, char **str, unsigned int word_count);
int		word_counter(char *input);
char	**get_token(char *input);
void	free_env_list(t_env *env);
char	ctrl_redir_continue(t_job *temp, char *arg, char *signal,
			char *arg_net);
void	assign_signal(char *arg, char *signal);
char	handle_distribution(t_job *temp, char *arg, char *signal);
char	append_files_order(t_job *temp, char *arg);
char	ctrl_redir(t_job *temp, char *arg, char *signal);
char	ctrl_redir_continue(t_job *temp, char *arg, char *signal,
			char *arg_net);
void	assign_signal(char *arg, char *signal);
int		check_redir_list(char **redir_list, char *arg_trimmed);
char	ctrl_append(t_redir *redir, char *arg);
int		init_new_job(t_job **temp, char **tokens);
char	fill_jobs_from_tokens(t_mshell *shell, char **tokens);
void	reset_job_list(t_mshell *mshell);
void	free_nec(t_mshell *mshell);
void	initialize_shell(t_mshell *mshell, char **env, t_env **env_list);
void	run_shell(t_mshell *mshell);
void	cleanup_shell(t_mshell *mshell);
int		process_user_input(char *input, t_mshell *mshell);
bool	is_special_char(const char *token);
bool	check_syntax_errors(char **tokens);
bool	check_unclosed_quotes(const char *input);

#endif
