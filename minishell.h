/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 13:38:02 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/08 19:09:31 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#define SPECIAL_CHARS "<>| \0"
#define QUOTES "'\""

#include <stdio.h>
#include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
#include <unistd.h>
#include "lib/libft/libft.h"
#include <stdbool.h>
#include <limits.h>
#include <fcntl.h>  // Dosya modları ve open() için gerekli
#include <signal.h>
# include "termios.h"
# include "termcap.h"
# include "sys/wait.h"

// typedef struct s_redirection
// {
//     char    *filename;      // Yönlendirilecek dosyanın adı veya heredoc delimiter
//     int     type;            // Yönlendirme türü: 0=<, 1=>, 2=>>, 3=<< (heredoc)
//     char    *delimiter;     // Sadece heredoc için delimiter
// } t_redirection;

// typedef struct s_command
// {
//     char                **args;        // Komut ve argümanları tutan array
//     t_redirection       *input_redirect;   // Giriş yönlendirmesi (örneğin '< input.txt')
//     t_redirection       *output_redirect;  // Çıkış yönlendirmesi (örneğin '> output.txt' veya '>> output.txt')
//     bool                append_mode;   // Output dosyası ekleme modunda mı (örneğin '>>')
//     bool                is_pipe;       // Pipe kullanımı var mı (örneğin '|')
//     struct s_command    *next;         // Bir sonraki komut (pipe için)
// } t_command;


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

// typedef struct s_quotes
// {
// 	bool	in_single_quotes;
// 	bool	in_double_quotes;
// }	t_quotes;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
	struct s_env		*prev;
}	t_env;

// typedef struct  s_cmd
// {
// 	char				**ncmd; // pipe'a göre bölünmüş inputu burda tutuyoruz.
// 	int					pipe_count;
// 	char				*cleaned; //executera gönderilecek double char pointer'ın tırnaklarının çıkartılmış hali
// 	char				**sep_path;//env'deki PATH'ın : işaretine göre split'lenmiş hali

// 	int					status;//$? için tutulan status
// 	int					**fd;//pipe fonksiyonunda açılan fd'leri tutacak
// 	char				**envp;//t_env'nin char **'a dönüştürülmüş hali

// 	t_env		        *env;
// 	t_env		        *exp;// export komutunda key'e atanmış herhangi bir value yoksa o keyi env'ye eklemez, exp structına ekler
// 	struct s_executor	*executor;
// }t_cmd;

// typedef struct s_redirect
// {
// 	int type; // 10: heredoc, 11: append, 12: input, 13: output
// 	char				*filename;
// 	struct s_redirect	*next;
// } t_redirection;

// typedef struct s_files
// {
// 	int					fd_heredoc[2];//açılan dosyaların fd'lerini tutar
// 	int					error;
// 	int					fd_input;
// 	int					fd_output;
// 	char				*input;
// 	char				*output;
// 	char				*heredoc;
// }						t_files;

// typedef struct s_executor
// {
// 	char				**argv;//execve'ye göndereceğimiz char **
// 	pid_t				pid;//child process'lerde execve çalıştırmak için
// 	t_files				*files;//redirection structı
// 	struct s_executor	*next;//pipe varsa yeni bir düğüm olarak kullanılır
// 	struct s_redirect	*redirect;//redirect'ın türünü ve adını tutan struct
// }						t_executor;

typedef struct s_job t_job;
typedef struct s_jobs t_jobs;
typedef struct s_redir t_redir;

typedef enum e_type
{
    NONE_TYPE,
    PIPE,
	EXEC
}   t_type;

typedef enum e_bool
{
    NONE_BOOL,
    OUT,
	APPEND
}   t_bool;

struct s_redir
{
    int     in_file;
    int     out_file;
	int		append_file;
	t_bool	last;
    char    **in_files;
	char	**out_files;
	char	**appends;
    char    **eof;
};

struct s_job
{
    char    *cmd;
	bool	is_builtin;
	pid_t	pid;
    char    **args;
    t_redir *redir;
    t_job   *next_job;
};

typedef struct s_mshell t_mshell;

struct s_jobs
{
    t_mshell    *mshell;
    t_type      type;
    t_job       *job_list;
    t_env       *env;  // t_env * olarak güncellendi
    int         len;
};

typedef struct termios t_termios;

struct s_mshell
{
    t_jobs      *jobs;
    t_termios   termios;
    int         status;
    char        is_exit;
    char        **success_arr;
	int			backup_fd[2];
};


void	handle_quotes(t_split *s, char *input);
void	handle_special_chars(t_split *s, char *input);
void	process_quotes_and_length(t_split *s, char *input);
void	handle_word_allocation(char *input, char **str, t_split *s, int word);

void	init_split(t_split *s);
int		skip_whitespace(char *input, int i, t_split *s);
char	**split_words(char *input, char **str, unsigned int word_count);

int		word_counter(char *input);
char	**get_token(char *input);
char	**free_array(char **ptr, int i);
char	*put_word(char *word, char *input, int start, int word_len);

int	pipe_cont(char *input);
int	redir_cont2(char *input, int i, char c);
int	redir_cont(char *input);
int	quote_cont(char *input);
int	syntax_cont(char *input, bool *has_error);

int	is_meta(char *input, int i);
int	is_space(char c);

int count_env_length(t_env *lst);
char **env_to_char_array(t_env *env_list);
void free_env_array(char **env_array);

t_env	**lstadd_back2(t_env **lst, t_env *new);
char	*funckey(char *env, int end);
char	*funcval(char *env, int start);
t_env	*envfunc2(char **env);
void	process_key(char **input_ptr, t_env *env, int *i, bool in_single_quotes);

void	check_quotes(char c, bool *sq, bool *dq);
void	replace_dollar_with_value_or_remove(char **input, char *value, int start, int end);
char	*get_env_value(t_env *env, char *key);
bool    contains_special_operators(char *key);
void	process_key(char **input_ptr, t_env *env, int *i, bool in_single_quotes);
void    get_dollar(char **input_ptr, t_jobs *jobs);

void	signal_handle_exec(t_mshell *mshell);

char 	**env_to_double_pointer(t_env *env_list);
char	heredoc(t_jobs *jobs, t_job *job);
char	executor(t_mshell *mshell);
void	free_str_arr(char **str_arr);
int 	str_arr_len(char **str_arr);
char 	**str_arr_realloc(char **str_arr, char *element);
char	*find_path(char *path, char *cmd);
void	is_builtin(t_job *job);
char	ctrl_builtins(t_jobs *jobs, t_job *job);
char	cd(t_mshell *mshell,char *path);
void	echo(char **args);
void	env(t_env *env);
void	exit_d(t_mshell *mshell, char **args);
char	export(t_env *env, t_mshell *mshell, char **args);
char	pwd(void);
void	unset(t_env *env, char **args);

#endif
