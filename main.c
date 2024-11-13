#include "minishell.h"

static char ctrl_redir(t_job *temp, char *arg, char *signal)
{
    char    *arg_net;

    if (*signal == 0)
    {
        *signal = -1;
        temp->redir->last_in = IN;
        arg_net = ft_strtrim(arg, "\"");
        temp->redir->in_files = str_arr_realloc(temp->redir->in_files, arg_net);
        if (!temp->redir->in_files)
            return (EXIT_FAILURE);
    }
    else if (*signal == 1)
    {
        *signal = -1;
        temp->redir->last_out = OUT;
        arg_net = ft_strtrim(arg, "\"");
        temp->redir->out_files = str_arr_realloc(temp->redir->out_files, arg_net);
        if (!temp->redir->out_files)
            return (EXIT_FAILURE);
    }
    else if (*signal == 2)
    {
        *signal = -1;
        temp->redir->last_in = HDOC;
        arg_net = ft_strtrim(arg, "\"");
        temp->redir->eof = str_arr_realloc(temp->redir->eof, arg_net);
        if (!temp->redir->eof)
            return (EXIT_FAILURE);
    }
    else if (*signal == 3)
    {
        *signal = -1;
        temp->redir->last_out = APPEND;
        arg_net = ft_strtrim(arg, "\"");
        temp->redir->appends = str_arr_realloc(temp->redir->appends, arg_net);
        if (!temp->redir->appends)
            return (EXIT_FAILURE);
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
        len_str = ft_strlen(redir->appends[len - 1]);
    if (redir->appends && !ft_strncmp(redir->appends[len - 1], arg_trimmed, len_str) && len_str == len_arg)
        return (1);
    len = str_arr_len(redir->in_files);
    if (redir->in_files)
        len_str = ft_strlen(redir->in_files[len - 1]);
    if (redir->in_files && !ft_strncmp(redir->in_files[len - 1], arg_trimmed, len_str) && len_str == len_arg)
        return (1);
    len = str_arr_len(redir->out_files);
    if (redir->out_files)
        len_str = ft_strlen(redir->out_files[len - 1]);
    if (redir->out_files && !ft_strncmp(redir->out_files[len - 1], arg_trimmed, len_str) && len_str == len_arg)
        return (1);
    len = str_arr_len(redir->eof);
    if (redir->eof)
        len_str = ft_strlen(redir->eof[len - 1]);
    if (redir->eof && !ft_strncmp(redir->eof[len - 1], arg_trimmed, len_str) && len_str == len_arg)
        return (1);
    return (0);
}

static char handle_distribution(t_job *temp, char *arg, char *signal)
{
    char    state;

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
        temp->args = str_arr_realloc(temp->args, arg);
        if (!temp->args)
            return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

static char fill_jobs_from_tokens(t_mshell *shell, char **tokens)
{
    t_job   *temp;
    //char    *arg;
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
    t_job   *temp1;

    temp1 = mshell->jobs->job_list;
    while (temp1)
    {
        free_job(temp1);
        temp1 = temp1->next_job;
    }
    mshell->jobs->job_list = NULL;
}

char number_of_quote(char *input)
{
    char    quote_type;
    bool    in_quote;
    int     i;

    i = 0;
    in_quote = false;
    while (input[i])
    {
        if (ft_strchr(QUOTES, input[i]))
        {
            if (!in_quote)
            {
            quote_type = input[i];
            in_quote = !in_quote;
            }
            else
            {
                if (input[i] == quote_type)
                    in_quote = !in_quote;
            }
        }
        i++;
    }
    if (in_quote)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
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
    g_exit_status = 0;// struct initleme işlemi başta yapılcak
    mshell.is_exit = 0;
    env_list = envfunc2(env);
    mshell.jobs = ft_calloc(1, sizeof(t_jobs));
    mshell.jobs->mshell = &mshell;
    mshell.jobs->env = env_list;
    while (1)
    {
        mshell.jobs->job_list = ft_calloc(1, sizeof(t_job));
        mshell.jobs->job_list->redir = ft_calloc(1, sizeof(t_redir));
        set_signal(MAIN_SF);
        input = readline("minishell> ");
        if (!input)
            break ;
        if (number_of_quote(input))
        {
            g_exit_status = 2;
            write(2, "Qutoes do not match\n", 21);
            continue ;
        }
        add_history(input);
        get_dollar(&input, mshell.jobs);
        if (!input[0])
            continue ;
        cmd = get_token(input);
        fill_jobs_from_tokens(&mshell, cmd);
        executor(&mshell);
        free_nec(&mshell);
        free(input);
    }
    return (0);
}
