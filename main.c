#include "minishell.h"

static char append_files_order(t_job *temp, char *arg)
{
    temp->redir->files_order = str_arr_realloc(temp->redir->files_order, arg);
    if (!temp->redir->files_order)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

static char ctrl_redir(t_job *temp, char *arg, char *signal)
{
    char    *arg_net;

    if (*signal == 0)
    {
        *signal = -1;
        temp->redir->last_in = IN;
        arg_net = ft_strtrim(arg, "\"");
        temp->redir->in_files = str_arr_realloc(temp->redir->in_files, arg_net);
        // free(arg_net);
        if (!temp->redir->in_files || append_files_order(temp, arg_net))
            return ((free(arg_net)),EXIT_FAILURE);
    }
    else if (*signal == 1)
    {
        *signal = -1;
        temp->redir->last_out = OUT;
        arg_net = ft_strtrim(arg, "\"");
        temp->redir->out_files = str_arr_realloc(temp->redir->out_files, arg_net);
        // free(arg_net);
        if (!temp->redir->out_files || append_files_order(temp, arg_net))
            return ((free(arg_net)),EXIT_FAILURE);
    }
    else if (*signal == 2)
    {
        *signal = -1;
        temp->redir->last_in = HDOC;
        arg_net = ft_strtrim(arg, "\"");
        temp->redir->eof = str_arr_realloc(temp->redir->eof, arg_net);
        // free(arg_net);
        if (!temp->redir->eof || append_files_order(temp, arg_net))
            return ((free(arg_net)),EXIT_FAILURE);
    }
    else if (*signal == 3)
    {
        *signal = -1;
        temp->redir->last_out = APPEND;
        arg_net = ft_strtrim(arg, "\"");
        temp->redir->appends = str_arr_realloc(temp->redir->appends, arg_net);
        // free(arg_net);
        if (!temp->redir->appends || append_files_order(temp, arg_net))
            return ((free(arg_net)),EXIT_FAILURE);
    }
    else
    {
        if (arg[0] == '<' && !arg[1])
            *signal = 0;
        else if (arg[0] == '>' && !arg[1])
            *signal = 1;
        else if (arg[0] == '<' && arg[1] == arg[0] && !arg[2])
            *signal = 2;
        else if (arg[0] == '>' && arg[1] == arg[0] && !arg[2])
            *signal = 3;
        else
            *signal = -1;
    }
    return (EXIT_SUCCESS);
}

static char ctrl_append(t_redir *redir, char *arg)
{
    char    *arg_trimmed;
    int     len_arg;
    int     len_str;
    int     len;

    arg_trimmed = ft_strtrim(arg, "\"");
    len_arg = ft_strlen(arg_trimmed);
    len = str_arr_len(redir->appends);
    if (redir->appends)
    {
        len_str = ft_strlen(redir->appends[len - 1]);
        if (!ft_strncmp(redir->appends[len - 1], arg_trimmed, len_str) && len_str == len_arg)
            return (free(arg_trimmed),1);
    }
    len = str_arr_len(redir->in_files);
    if (redir->in_files)
    {
        if (redir->in_files)
            len_str = ft_strlen(redir->in_files[len - 1]);
        if (!ft_strncmp(redir->in_files[len - 1], arg_trimmed, len_str) && len_str == len_arg)
            return (free(arg_trimmed),1);
    }
    len = str_arr_len(redir->out_files);
    if (redir->out_files)
    {
        len_str = ft_strlen(redir->out_files[len - 1]);
        if (!ft_strncmp(redir->out_files[len - 1], arg_trimmed, len_str) && len_str == len_arg)
            return (free(arg_trimmed),1);
    }
    len = str_arr_len(redir->eof);
    if (redir->eof)
    {
        len_str = ft_strlen(redir->eof[len - 1]);
        if (!ft_strncmp(redir->eof[len - 1], arg_trimmed, len_str) && len_str == len_arg)
            return (free(arg_trimmed),1);
    }
    free(arg_trimmed);
    return (0);
}

void    free_double_pointer(char **str)
{
    int i;

    i = 0;
    while (str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}

static char handle_distribution(t_job *temp, char *arg, char *signal)
{
    char    state;
    char **a;

    a = temp->args;
    if (!*arg)
        return (EXIT_FAILURE);
    state = ctrl_redir(temp, arg, signal);
    if (arg[0] == '<' && !arg[1])
        return (EXIT_SUCCESS);
    else if (arg[0] == '>' && !arg[1])
        return (EXIT_SUCCESS);
    else if (arg[0] == '<' && arg[1] == arg[0] && !arg[2])
        return (EXIT_SUCCESS);
    else if (arg[0] == '>' && arg[1] == arg[0] && !arg[2])
        return (EXIT_SUCCESS);
    if (state == EXIT_FAILURE)
        return (EXIT_FAILURE);
    else if (*signal == -1 && !ctrl_append(temp->redir, arg))
    {
        temp->args = str_arr_realloc(a, arg);
        if (!temp->args)
            return  (free_double_pointer(temp->args), EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

static char fill_jobs_from_tokens(t_mshell *shell, char **tokens)
{
    t_job   *temp;
    char    signal;
    int     i;

    signal = -1;
    shell->jobs->len = 1;
    temp = shell->jobs->job_list;
    i = 0;
    while (tokens[i])
    {
        if (tokens[i][0] == '|')
        {
            shell->jobs->len += 1;
            temp->next_job = ft_calloc(1, sizeof(t_job));
            if (!temp->next_job)
                return (free_str_arr(tokens), EXIT_FAILURE);
            temp = temp->next_job;
            temp->redir = ft_calloc(1, sizeof(t_redir));
            if (!temp->redir)
                return (free_str_arr(tokens), EXIT_FAILURE);
            temp->redir->append_file = -1;
            temp->redir->in_file = -1;
            temp->redir->out_file = -1;
            temp->redir->last_in = NONE_BOOL;
            temp->redir->last_out = NONE_BOOL;
        }
        else if (handle_distribution(temp, tokens[i], &signal))
            return (free_str_arr(tokens), EXIT_FAILURE);
        i++;
    }
    return (EXIT_SUCCESS);
}

static void free_nec(t_mshell *mshell)
{
    t_job *temp1;
    t_job *next;

    temp1 = mshell->jobs->job_list;
    while (temp1)
    {
        next = temp1->next_job; // temp1 serbest bırakılmadan önce next_job'u al
        free_job(temp1);        // temp1'i güvenle serbest bırak
        temp1 = next;           // Sonraki düğüme geç
    }
    mshell->jobs->job_list = NULL;
}

bool is_special_char(const char *token)
{
    return (ft_strncmp(token, "|", 2) == 0 ||
            ft_strncmp(token, "<", 2) == 0 ||
            ft_strncmp(token, ">", 2) == 0 ||
            ft_strncmp(token, "<<", 3) == 0 ||
            ft_strncmp(token, ">>", 3) == 0);
}

bool check_syntax_errors(char **tokens)
{
    int i = 0;

    while (tokens[i])
    {
        // Pipe kontrolü
        if (ft_strncmp(tokens[i], "|", 2) == 0)
        {
            if (i == 0 || tokens[i + 1] == NULL || is_special_char(tokens[i + 1]))
            {
                ft_putendl_fd("Syntax error near unexpected token '|'", 2);
                return true;
            }
        }

        // Redirection kontrolü
        if (is_special_char(tokens[i]) && ft_strncmp(tokens[i], "|", 2) != 0)
        {
            if (tokens[i + 1] == NULL || is_special_char(tokens[i + 1]))
            {
                ft_putendl_fd("Syntax error near unexpected token", 2);
                return true;
            }
        }

        i++;
    }

    return false;
}

bool check_unclosed_quotes(const char *input)
{
    char quote = '\0';

    while (*input)
    {
        if ((*input == '\'' || *input == '"') && (quote == '\0' || quote == *input))
            quote = (quote == '\0') ? *input : '\0'; // Aç/kapa
        input++;
    }

    if (quote != '\0')
    {
        ft_putendl_fd("Syntax error: Unclosed quote", 2);
        return true;
    }

    return false;
}

// int main(int argc, char **argv, char **env)
// {
//     char *input;
//     t_env *env_list;
//     t_mshell mshell;
//     char **cmd;

//     (void)argv;
//     if (argc != 1)
//         return 1;
//     g_exit_status = 0;// struct initleme işlemi başta yapılcak
//     mshell.is_exit = 0;
//     env_list = envfunc2(env);
//     mshell.jobs = ft_calloc(1, sizeof(t_jobs));
//     mshell.jobs->mshell = &mshell;
//     mshell.jobs->env = env_list;
//     while (1)
//     {
//         mshell.jobs->job_list = ft_calloc(1, sizeof(t_job));
//         mshell.jobs->job_list->redir = ft_calloc(1, sizeof(t_redir));
//         set_signal(MAIN_SF);
//         input = readline("minishell> ");
//         if (!input)
//             break ;
//         // if(check_syntax_errors(input))
//         // {
//         //     free(input);
//         //     continue;
//         // }
//         set_signal(314159);
//         add_history(input);
//         get_dollar(&input, mshell.jobs);
//         if (!input[0])
//             continue ;
//         cmd = get_token(input);
//         fill_jobs_from_tokens(&mshell, cmd);
//         executor(&mshell);
//         free_nec(&mshell);
//         free(input);
//     }
//     return (0);
// }



void free_env_list(t_env *env)
{
    t_env *temp;

    while (env)
    {
        temp = env->next;    // Sonraki düğümü sakla
        if (env->key)
            free(env->key);  // `key` belleğini serbest bırak
        if (env->value)
            free(env->value); // `value` belleğini serbest bırak
        free(env);           // Mevcut düğümün kendisini serbest bırak
        env = temp;          // Sonraki düğüme geç
    }
}



int main(int argc, char **argv, char **env)
{
    char *input;
    t_env *env_list;
    t_mshell mshell;
    char **cmd;

    (void)argv;
    if (argc != 1)
        return 1;

    // Başlangıç ayarları
    g_exit_status = 0;
    mshell.is_exit = 0;
    env_list = envfunc2(env);
    mshell.jobs = ft_calloc(1, sizeof(t_jobs));
    mshell.jobs->mshell = &mshell;
    mshell.jobs->env = env_list;

    while (1)
    {
        if (mshell.jobs->job_list)
            free_nec(&mshell);
        mshell.jobs->job_list = ft_calloc(1, sizeof(t_job));
        mshell.jobs->job_list->redir = ft_calloc(1, sizeof(t_redir));

        set_signal(MAIN_SF);
        input = readline("minishell> ");
        if (!input)
        {
            free(input);
            break;
        }
        set_signal(314159);
        add_history(input);
        get_dollar(&input, mshell.jobs);
        if (!input[0])
        {
            free(input);
            continue;
        }
        if (check_unclosed_quotes(input))
        {
            continue;
            return 1;
        }
        cmd = get_token(input);
        if (!cmd)
        {
            free(input);
            continue;
        }
         if (check_syntax_errors(cmd))
        {
            continue;
            return 1;
        }
        if (fill_jobs_from_tokens(&mshell, cmd) == EXIT_FAILURE)
        {
            free(input);
            free_str_arr(cmd);
            continue;
        }
        executor(&mshell);
        free_str_arr(cmd);
        free(input);
    }
    free_jobs_list(mshell.jobs->job_list);
    free_env_list(mshell.jobs->env);
    free(mshell.jobs);

    return (0);
}
