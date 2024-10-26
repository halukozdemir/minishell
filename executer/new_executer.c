#include "../minishell.h"

void add_job_to_jobs(t_jobs *jobs, t_job *new_job)
{
    if (!jobs->job_list)
    {
        jobs->job_list = new_job;
    }
    else
    {
        t_job *current = jobs->job_list;
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

    shell->jobs = malloc(sizeof(t_jobs));
    if (!shell->jobs)
    {
        return;
    }

    shell->jobs->type = NONE;
    shell->jobs->job_list = NULL;
    shell->jobs->len = 0;
    shell->jobs->pipe[0] = -1;
    shell->jobs->pipe[1] = -1;

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
            return;
        }

        current_job->cmd = ft_strdup(tokens[i++]);

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
            return;
        }

        current_job->args[0] = ft_strdup(current_job->cmd);
        int j = 1;
        for (int k = arg_start; k < arg_start + arg_count; k++)
        {
            current_job->args[j++] = ft_strdup(tokens[k]);
        }
        current_job->args[j] = NULL;

        current_job->redir = malloc(sizeof(t_redir));
        if (!current_job->redir)
        {
            free(current_job->args);
            free(current_job);
            return;
        }

        current_job->redir->in_file = -1;
        current_job->redir->out_file = -1;
        current_job->redir->files = NULL;
        current_job->redir->eof = NULL;
        current_job->redir->args = NULL;

        while (tokens[i] && (ft_strncmp(tokens[i], ">", 1) == 0 || ft_strncmp(tokens[i], "<", 1) == 0 || ft_strncmp(tokens[i], ">>", 2) == 0 || ft_strncmp(tokens[i], "<<", 2) == 0))
        {
            if (ft_strncmp(tokens[i], ">", 1) == 0)
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
                }
            }
            else if (ft_strncmp(tokens[i], ">>", 2) == 0)
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
                }
            }
            else if (ft_strncmp(tokens[i], "<<", 2) == 0)
            {
                i++;
                if (tokens[i])
                {
                    current_job->redir->eof = ft_strdup(tokens[i]);
                }
            }
            i++;
        }

        current_job->next_job = NULL;
        add_job_to_jobs(shell->jobs, current_job);
    }
}

void execute_jobs(t_jobs *jobs)
{
    t_job *current_job = jobs->job_list;
    int prev_pipe[2] = {-1, -1};
    int curr_pipe[2];

    while (current_job)
    {
        if (jobs->type == PIPE && current_job->next_job)
        {
            if (pipe(curr_pipe) == -1)
            {
                perror("pipe error");
                return;
            }
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork error");
            return;
        }
        else if (pid == 0)
        {
            if (current_job->redir->in_file != -1)
            {
                dup2(current_job->redir->in_file, STDIN_FILENO);
                close(current_job->redir->in_file);
            }
            if (current_job->redir->out_file != -1)
            {
                dup2(current_job->redir->out_file, STDOUT_FILENO);
                close(current_job->redir->out_file);
            }
            if (prev_pipe[0] != -1)
            {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
            }
            if (jobs->type == PIPE && current_job->next_job)
            {
                close(curr_pipe[0]);
                dup2(curr_pipe[1], STDOUT_FILENO);
                close(curr_pipe[1]);
            }

            execvp(current_job->cmd, current_job->args);
            perror("execvp error");
            exit(EXIT_FAILURE);
        }
        else
        {
            if (prev_pipe[0] != -1)
                close(prev_pipe[0]);
            if (prev_pipe[1] != -1)
                close(prev_pipe[1]);

            if (jobs->type == PIPE && current_job->next_job)
            {
                prev_pipe[0] = curr_pipe[0];
                prev_pipe[1] = curr_pipe[1];
                close(curr_pipe[1]);
            }
        }

        current_job = current_job->next_job;
    }

    current_job = jobs->job_list;
    while (current_job)
    {
        wait(NULL);
        current_job = current_job->next_job;
    }
}

void free_jobs(t_jobs *jobs)
{
    t_job *current_job = jobs->job_list;
    t_job *next_job;

    while (current_job)
    {
        next_job = current_job->next_job;
        free(current_job->cmd);
        int i = 0;
        while (current_job->args[i])
        {
            free(current_job->args[i]);
            i++;
        }
        free(current_job->args);
        if (current_job->redir)
        {
            if (current_job->redir->in_file != -1)
                close(current_job->redir->in_file);
            if (current_job->redir->out_file != -1)
                close(current_job->redir->out_file);
            free(current_job->redir->eof);
            free(current_job->redir);
        }
        free(current_job);
        current_job = next_job;
    }
    free(jobs);
}
