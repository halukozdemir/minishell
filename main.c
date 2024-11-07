#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void add_job_to_jobs(t_jobs *jobs, t_job *new_job)
{
    t_job *current;

    // printf("Adding job to jobs list...\n");
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
    // printf("Job added. Current job list length: %d\n", jobs->len);
}

void fill_jobs_from_tokens(t_mshell *shell, char **tokens)
{
    int i = 0;
    t_job *current_job = NULL;

    shell->jobs->type = NONE;
    shell->jobs->job_list = NULL;
    shell->jobs->len = 0;
    shell->jobs->active_pipe[0] = -1;
    shell->jobs->active_pipe[1] = -1;
    shell->jobs->old_pipe[0] = -1;
    shell->jobs->old_pipe[1] = -1;
    while (tokens[i])
    {
        // printf("Processing token: %s\n", tokens[i]);
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
        // printf("Command set: %s\n", current_job->cmd);

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
        current_job->redir->files = NULL;
        current_job->redir->eof = NULL;
        current_job->redir->args = NULL;

        while (tokens[i] && (ft_strncmp(tokens[i], ">", 1) == 0 || ft_strncmp(tokens[i], "<", 1) == 0 || ft_strncmp(tokens[i], ">>", 2) == 0 || ft_strncmp(tokens[i], "<<", 2) == 0))
        {
            if (ft_strncmp(tokens[i], ">>", 2) == 0)
            {
                i++;
                if (tokens[i])
                {
                    current_job->redir->out_file = open(tokens[i], O_CREAT | O_WRONLY | O_APPEND, 0644);
                    if (current_job->redir->out_file == -1)
                    {
                        perror("open error");
                        free(current_job->redir);
                        free(current_job->args);
                        free(current_job);
                        return;
                    }
                    // printf("Appending output redirection to file: %s\n", tokens[i]);
                }
            }
            else if (ft_strncmp(tokens[i], "<<", 2) == 0)
            {
                i++;
                if (tokens[i])
                {
                    current_job->redir->eof = str_arr_realloc(current_job->redir->eof, ft_strdup(tokens[i]));
                    //printf("Heredoc EOF set: %s\n", tokens[i]);
                }
            }
            // printf("Processing redirection: %s\n", tokens[i]);
            else if (ft_strncmp(tokens[i], ">", 1) == 0)
            {
                i++;
                if (tokens[i])
                {
                    current_job->redir->out_file = open(tokens[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    if (current_job->redir->out_file == -1)
                    {
                        perror("open error");
                        free(current_job->redir);
                        free(current_job->args);
                        free(current_job);
                        return;
                    }
                    // printf("Output redirection set to file: %s\n", tokens[i]);
                }
            }
            else if (ft_strncmp(tokens[i], "<", 1) == 0)
            {
                i++;
                if (tokens[i])
                {
                    current_job->redir->in_file = open(tokens[i], O_RDONLY);
                    if (current_job->redir->in_file == -1)
                    {
                        perror("open error");
                        free(current_job->redir);
                        free(current_job->args);
                        free(current_job);
                        return;
                    }
                    // printf("Input redirection set to file: %s\n", tokens[i]);
                }
            }
            i++;
        }

        current_job->next_job = NULL;
        add_job_to_jobs(shell->jobs, current_job);
    }
}


// #include <stdio.h>

// void print_debug_redir(t_redir *redir)
// {
//     if (!redir)
//     {
//         printf("No redirection.\n");
//         return;
//     }
//     printf("Redirection:\n");
//     printf("  In file: %d\n", redir->in_file);
//     printf("  Out file: %d\n", redir->out_file);
//     printf("  EOF: %s\n", redir->eof ? redir->eof : "NULL");
//     printf("  Args: %s\n", redir->args ? redir->args : "NULL");
//     if (redir->files)
//     {
//         int i = 0;
//         printf("  Files:\n");
//         while (redir->files[i])
//         {
//             printf("    File[%d]: %s\n", i, redir->files[i]);
//             i++;
//         }
//     }
//     else
//     {
//         printf("  No additional files.\n");
//     }
// }

// void print_debug_job(t_job *job)
// {
//     while (job)
//     {
//         printf("Job:\n");
//         printf("  Command: %s\n", job->cmd);

//         if (job->args)
//         {
//             int i = 0;
//             printf("  Arguments:\n");
//             while (job->args[i])
//             {
//                 printf("    Arg[%d]: %s\n", i, job->args[i]);
//                 i++;
//             }
//         }
//         else
//         {
//             printf("  No arguments.\n");
//         }

//         print_debug_redir(job->redir);

//         job = job->next_job;
//     }
// }

// void print_debug_jobs(t_jobs *jobs)
// {
//     if (!jobs)
//     {
//         printf("No jobs to display.\n");
//         return;
//     }

//     printf("Jobs structure:\n");
//     printf("  Type: %d\n", jobs->type);
//     printf("  Length: %d\n", jobs->len);
//     printf("  Pipe[0]: %d, Pipe[1]: %d\n", jobs->pipe[0], jobs->pipe[1]);

//     if (jobs->job_list)
//     {
//         print_debug_job(jobs->job_list);
//     }
//     else
//     {
//         printf("  No job list.\n");
//     }
// }

// void print_debug_mshell(t_mshell *shell)
// {
//     if (!shell)
//     {
//         printf("Shell structure is NULL.\n");
//         return;
//     }

//     printf("Mshell structure:\n");
//     print_debug_jobs(shell->jobs);

//     if (shell->success_arr)
//     {
//         int i = 0;
//         printf("Success Array:\n");
//         while (shell->success_arr[i])
//         {
//             printf("  Success[%d]: %s\n", i, shell->success_arr[i]);
//             i++;
//         }
//     }
//     else
//     {
//         printf("No success array.\n");
//     }

//     printf("Env struct: %p\n", shell->env);  // Assuming env will be printed elsewhere
// }

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
	rl_replace_line("", 0);
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
    char    state;
    int     i;

    state = 0;
    i = 0;
    while (input[i])
    {
        if (!state && !ft_strchr(QUOTES, input[i]))
            state = input[i];
        else if (state && !ft_strchr(QUOTES, input[i]))
            state = 0;
        i++;
    }
    if (state)
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

    // env'i t_env structına dönüştürme
    env_list = envfunc2(env);
    mshell.jobs = ft_calloc(1, sizeof(t_jobs));
    mshell.jobs->mshell = &mshell;
    mshell.jobs->env = env_list;
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break ;
        if (!number_of_quote(input))
        {
            mshell.status = 2;
            continue ;// error fd = 2
        }
        add_history(input);
        get_dollar(&input, mshell.jobs);
        cmd = get_token(input);
        //int g = -1;
        // while (cmd[++g])
        //     printf("%s\n", cmd[g]);
        fill_jobs_from_tokens(&mshell, cmd);
        executor(&mshell);

        free(input);
    }
    return 0;
}
