/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 13:38:02 by halozdem          #+#    #+#             */
/*   Updated: 2024/10/23 17:10:26 by halozdem         ###   ########.fr       */
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

typedef struct s_redirection
{
    char    *filename;      // Yönlendirilecek dosyanın adı veya heredoc delimiter
    int     type;            // Yönlendirme türü: 0=<, 1=>, 2=>>, 3=<< (heredoc)
    char    *delimiter;     // Sadece heredoc için delimiter
} t_redirection;

typedef struct s_command
{
    char                **args;        // Komut ve argümanları tutan array
    t_redirection       *input_redirect;   // Giriş yönlendirmesi (örneğin '< input.txt')
    t_redirection       *output_redirect;  // Çıkış yönlendirmesi (örneğin '> output.txt' veya '>> output.txt')
    bool                append_mode;   // Output dosyası ekleme modunda mı (örneğin '>>')
    bool                is_pipe;       // Pipe kullanımı var mı (örneğin '|')
    struct s_command    *next;         // Bir sonraki komut (pipe için)
} t_command;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
	struct s_env		*prev;
}	t_env;


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
void handle_redirection(char *filename, int redir_type);
t_command *create_command(char **tokens);


#endif
