#include "minishell.h"
#include <string.h>

void add_job_to_jobs(t_jobs *jobs, t_job *new_job)
{
    t_job *current;

    if (!jobs->job_list)
        jobs->job_list = new_job;
    else
    {
        current = jobs->job_list;
        while (current->next_job)
            current = current->next_job;
        current->next_job = new_job;
    }
    jobs->len++;
}

void fill_jobs_from_tokens(t_mshell *shell, char **tokens)
{
    int i = 0;
    t_job *current_job = NULL;

    shell->jobs->job_list = NULL;
    shell->jobs->len = 0;
    while (tokens[i])
    {
        if (ft_strncmp(tokens[i], "|", 1) == 0)
        {
            shell->jobs->type = PIPE;
            i++;
            continue;
        }

        current_job = malloc(sizeof(t_job));
        if (!current_job)
        {
            printf("Memory allocation failed for current job\n");
            return;
        }

        current_job->cmd = ft_strdup(tokens[i++]);
        current_job->redir = ft_calloc(1, sizeof(t_redir));
        if (!current_job->redir)
        {
            free(current_job->args);
            free(current_job);
            printf("Memory allocation failed for redirection\n");
            return;
        }
        int arg_count = 0;
        int arg_start = i;
        while (tokens[i] && ft_strncmp(tokens[i], "|", 1) != 0 && ft_strncmp(tokens[i], ">", 1) != 0 && ft_strncmp(tokens[i], "<", 1) != 0 && ft_strncmp(tokens[i], ">>", 2) != 0 && ft_strncmp(tokens[i], "<<", 2) != 0)
        {
            arg_count++;
            i++;
        }

        current_job->args = malloc(sizeof(char *) * (arg_count + 2)); // +2: cmd + NULL
        if (!current_job->args)
        {
            free(current_job);
            printf("Memory allocation failed for arguments\n");
            return;
        }

        current_job->args[0] = ft_strdup(current_job->cmd);
        int j = 1;
        for (int k = arg_start; k < arg_start + arg_count; k++)
        {
            current_job->args[j++] = ft_strdup(tokens[k]);
            // printf("Argument added: %s\n", tokens[k]);
        }
        current_job->args[j] = NULL;
        current_job->redir->in_file = -1;
        current_job->redir->out_file = -1;
		current_job->redir->append_file = -1;
        current_job->redir->in_files = NULL;
        current_job->redir->out_files = NULL;
		current_job->redir->appends = NULL;
        current_job->redir->eof = NULL;
		current_job->redir->last_out = NONE_BOOL;
        current_job->redir->last_in = NONE_BOOL;
        while (tokens[i] && (ft_strncmp(tokens[i], ">", 1) == 0 || ft_strncmp(tokens[i], "<", 1) == 0 || ft_strncmp(tokens[i], ">>", 2) == 0 || ft_strncmp(tokens[i], "<<", 2) == 0))
        {
            if (ft_strncmp(tokens[i], ">>", 2) == 0)
            {
                i++;
                if (tokens[i])
                {
                    current_job->redir->appends = str_arr_realloc(current_job->redir->appends, ft_strdup(tokens[i]));
					current_job->redir->last_out = APPEND;
                }
            }
            else if (ft_strncmp(tokens[i], "<<", 2) == 0)
            {
                i++;
                if (tokens[i])
                {
                    current_job->redir->eof = str_arr_realloc(current_job->redir->eof, ft_strdup(tokens[i]));
                    current_job->redir->last_in = HDOC;
                }
            }
            else if (ft_strncmp(tokens[i], ">", 1) == 0)
            {
                i++;
                if (tokens[i])
                {
                    current_job->redir->out_files = str_arr_realloc(current_job->redir->out_files, ft_strdup(tokens[i]));
					current_job->redir->last_out = OUT;
                }
            }
            else if (ft_strncmp(tokens[i], "<", 1) == 0)
            {
                i++;
                if (tokens[i])
                {
                    current_job->redir->in_files = str_arr_realloc(current_job->redir->in_files, ft_strdup(tokens[i]));
                    current_job->redir->last_in = IN;
                }
            }
            i++;
        }
        current_job->next_job = NULL;
        add_job_to_jobs(shell->jobs, current_job);
    }
}


static void	ctrl_output(t_mshell *mshell, char state)
{
	if (state == 1)
	{
		tcgetattr(STDIN_FILENO, &mshell->termios);
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &mshell->termios);
		return ;
	}
	else
	{
		tcgetattr(STDIN_FILENO, &mshell->termios);
		mshell->termios.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &mshell->termios);
		mshell->termios.c_lflag |= ECHOCTL;
	}
}



static void	reset_prompt(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	//rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void	reset_prompt_exec(int signal)
{
	(void)signal;
	write(1, "\n", 1);
}

void	signal_handle_general(t_mshell *mshell)
{
	signal(SIGQUIT, &reset_prompt);
	signal(SIGINT, &reset_prompt);
	ctrl_output(mshell, 1);
}

void	signal_handle_exec(t_mshell *mshell)
{
	signal(SIGQUIT, &reset_prompt);
	signal(SIGINT, &reset_prompt_exec);
	ctrl_output(mshell, 0);
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
    mshell.status = 0;// struct initleme işlemi başta yapılcak
    mshell.is_exit = 0;
    // env'i t_env structına dönüştürme
    env_list = envfunc2(env);
    mshell.jobs = ft_calloc(1, sizeof(t_jobs));
    mshell.jobs->mshell = &mshell;
    mshell.jobs->env = env_list;
    while (1)
    {
		signal_handle_general(&mshell);
        input = readline("minishell> ");
        if (!input)
            break ;
        if (number_of_quote(input))
        {
            mshell.status = 2;
            continue ;// error fd = 2
        }
        add_history(input);
        get_dollar(&input, mshell.jobs);
        cmd = get_token(input);
        fill_jobs_from_tokens(&mshell, cmd);
        executor(&mshell);
        free(input);
    }
    return (0);
}
