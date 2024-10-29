/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 13:38:02 by halozdem          #+#    #+#             */
/*   Updated: 2024/10/29 18:42:43 by halozdem         ###   ########.fr       */
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
#include <limits.h>
#include <fcntl.h>  // Dosya modları ve open() için gerekli

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
typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
	struct s_env		*prev;
}	t_env;

typedef struct  s_cmd
{
	char				**ncmd; // pipe'a göre bölünmüş inputu burda tutuyoruz.
	int					pipe_count;
	char				*cleaned; //executera gönderilecek double char pointer'ın tırnaklarının çıkartılmış hali
	char				**sep_path;//env'deki PATH'ın : işaretine göre split'lenmiş hali

	int					status;//$? için tutulan status
	int					**fd;//pipe fonksiyonunda açılan fd'leri tutacak
	char				**envp;//t_env'nin char **'a dönüştürülmüş hali

	t_env		        *env;
	t_env		        *exp;// export komutunda key'e atanmış herhangi bir value yoksa o keyi env'ye eklemez, exp structına ekler
	struct s_executor	*executor;
}t_cmd;

typedef struct s_redirect
{
	int type; // 10: heredoc, 11: append, 12: input, 13: output
	char				*filename;
	struct s_redirect	*next;
} t_redirection;

typedef struct s_files
{
	int					fd_heredoc[2];//açılan dosyaların fd'lerini tutar
	int					error;
	int					fd_input;
	int					fd_output;
	char				*input;
	char				*output;
	char				*heredoc;
}						t_files;

typedef struct s_executor
{
	char				**argv;//execve'ye göndereceğimiz char **
	pid_t				pid;//child process'lerde execve çalıştırmak için
	t_files				*files;//redirection structı
	struct s_executor	*next;//pipe varsa yeni bir düğüm olarak kullanılır
	struct s_redirect	*redirect;//redirect'ın türünü ve adını tutan struct
}						t_executor;


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
			char *value, int start, int end, bool needs_quotes);
char	*get_env_value(t_env *env, char *key);void	process_key(char **input_ptr, t_env *env, int *i, bool in_single_quotes);
void	get_dollar(char **input_ptr, t_env *env);

#endif
