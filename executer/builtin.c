#include "../minishell.h"


// Check if command is a built-in and execute the corresponding function
int handle_builtin(char **args, t_mshell *shell)
{
    if (ft_strncmp(args[0], "echo", 4) == 0)
    {
        echo_builtin(args);
        return 1;
    }
    else if (ft_strncmp(args[0], "cd", 2) == 0)
    {
        cd_builtin(args);
        return 1;
    }
    else if (ft_strncmp(args[0], "pwd", 3) == 0)
    {
        pwd_builtin();
        return 1;
    }
    else if (ft_strncmp(args[0], "export", 6) == 0)
    {
        export_builtin(args);
        return 1;
    }
    else if (ft_strncmp(args[0], "unset", 5) == 0)
    {
        unset_builtin(args);
        return 1;
    }
    else if (ft_strncmp(args[0], "env", 3) == 0)
    {
        env_builtin(shell->env);
        return 1;
    }
    else if (ft_strncmp(args[0], "exit", 4) == 0)
    {
        exit_builtin();
        return 1;
    }
    return 0;
}

// Built-in function definitions
void echo_builtin(char **args)
{
    int i = 1;
    int newline = 1;

    if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
    {
        newline = 0;
        i = 2;
    }

    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
}

void cd_builtin(char **args)
{
    if (!args[1] || chdir(args[1]) != 0)
    {
        perror("cd error");
    }
}

void pwd_builtin()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("pwd error");
    }
}

void export_builtin(char **args)
{
    // Simple implementation for demo purposes
    if (args[1])
    {
        putenv(args[1]);
    }
    else
    {
        printf("export: not enough arguments\n");
    }
}

void unset_builtin(char **args)
{
    if (args[1])
    {
        unsetenv(args[1]);
    }
    else
    {
        printf("unset: not enough arguments\n");
    }
}

void env_builtin(char **env)
{
    int i = 0;
    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
}

void exit_builtin()
{
    printf("Exiting minishell...\n");
    exit(0);
}

// Modified execute_jobs function to handle built-ins
void execute_jobs(t_jobs *jobs, t_mshell *shell)
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

        if (handle_builtin(current_job->args, shell))
        {
            // Built-in command was executed, no need to fork
            current_job = current_job->next_job;
            continue;
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
