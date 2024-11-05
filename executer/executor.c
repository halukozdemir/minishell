#include "../minishell.h"

char **env_to_double_pointer(t_env *env_list)
{
    int count = 0;
    t_env *temp = env_list;

    // Önce kaç tane env değişkeni olduğunu sayalım
    while (temp)
    {
        count++;
        temp = temp->next;
    }

    // Double pointer dizisini oluştur
    char **env_array = malloc(sizeof(char *) * (count + 1)); // +1 for NULL terminator
    if (!env_array)
        return NULL;

    temp = env_list;
    int i = 0;
    while (i < count)
    {
        // "key=value" formatında birleştir
        int len = ft_strlen(temp->key) + ft_strlen(temp->value) + 2; // +1 for '=' and +1 for '\0'
        env_array[i] = malloc(len);
        if (!env_array[i])
        {
            // Bellek tahsis hatası durumunda, daha önce ayrılmış bellekleri serbest bırak
            int j = 0;
            while (j < i)
            {
                free(env_array[j]);
                j++;
            }
            free(env_array);
            return NULL;
        }
        temp = temp->next;
        i++;
    }
    env_array[count] = NULL; // Dizi sonlandırıcısı

    return env_array;
}

static void	close_pipe(int pipes[2])
{
	close(pipes[0]);
	close(pipes[1]);
}

static void handle_redirects(t_redir *redir)
{
    if (redir->in_file != -1)
    {
        if (dup2(redir->in_file, STDIN_FILENO) == -1)
            perror("dup2 in_file");
        close(redir->in_file);
    }
    if (redir->out_file != -1)
    {
        if (dup2(redir->out_file, STDOUT_FILENO) == -1)
            perror("dup2 out_file");
        close(redir->out_file);
    }
    if (redir->eof)
    {
        // Heredoc işlemi için bir pipe açıp yazılacak içerikleri oraya göndereceğiz
        int heredoc_pipe[2];
        if (pipe(heredoc_pipe) == -1)
        {
            perror("pipe");
            return;
        }

        write(heredoc_pipe[1], redir->eof, ft_strlen(redir->eof));
        close(heredoc_pipe[1]);

        if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
            perror("dup2 heredoc");
        close(heredoc_pipe[0]);
    }
}

static void exec_child(int i, t_jobs *jobs, char **envp)
{
    char    *temp;
    t_job   *temp_job;

    if (i > 0)
        dup2(jobs->old_pipe[0], STDIN_FILENO);
    if (i + 1 < jobs->len)
        dup2(jobs->active_pipe[1], STDOUT_FILENO);
    if (i > 0)
        close_pipe(jobs->old_pipe);
    if (i + 1 < jobs->len)
        close_pipe(jobs->active_pipe);

    temp_job = jobs->job_list;
    while (i)
    {
        temp_job = temp_job->next_job;
        i--;
    }

    // Redirect işlemlerini yap
    handle_redirects(temp_job->redir);

    temp = find_path(get_env_value(jobs->env, "PATH"), temp_job->cmd);
    if (!temp)
    {
        printf("exec error\n");
        return ;
    }
    execve(temp, temp_job->args, envp);
    perror("execve");
}

char executor(t_jobs *jobs)
{
    int     pipes[jobs->len - 1][2];
    pid_t   pid;
    int     i;

    i = -1;
    while (++i < jobs->len)
    {
        jobs->job_list->status = 0;
        if (i + 1 < jobs->len)
        {
            if (pipe(jobs->active_pipe) == -1)
                return (perror("pipe"), EXIT_FAILURE);
        }
        pid = fork();
        if (pid == -1)
            return (perror("fork"), EXIT_FAILURE);
        if (pid == 0)
            exec_child(i, jobs, env_to_double_pointer(jobs->env));
        if (i > 0)
            close_pipe(jobs->old_pipe);
        if (i + 1 < jobs->len)
        {
            jobs->old_pipe[0] = jobs->active_pipe[0];
            jobs->old_pipe[1] = jobs->active_pipe[1];
        }
    }
    i = -1;
    while (++i < jobs->len)
        wait(NULL);
    return (EXIT_SUCCESS);
}
