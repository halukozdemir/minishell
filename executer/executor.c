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
	char	*temp1;
	char	*temp2;
    int i = 0;
    while (i < count)
    {
		temp1 = ft_strdup(temp->key);
		temp2 = temp1;
		temp1 = ft_strjoin(temp1, "=");
		free(temp2);
		env_array[i] = ft_strjoin(temp1, temp->value);
		free(temp1);
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

char	heredoc(t_jobs *jobs, t_job *job, char state)
{
	char	*buffer;
	int		temp_status;
	int		pipe_fd[2];
	int		len1;
	int		len2;
    int     i;

	if (pipe(pipe_fd) == -1)
		return (EXIT_FAILURE);
	job->pid = fork();
    if (job->pid == 0)
    {
		if (state)
			//set_signal(HEREDOC_P);
		dup2(jobs->mshell->backup_fd[0], 0);
        i = 0;
        while (job->redir->eof[i])
		{
			/*
			if (jobs->mshell->status == 999)
			{
				free(buffer);
				exit(130);
			}
			*/
			buffer = readline(">");
			/*
			if (!buffer)
			{
				jobs->mshell->status = 130;
				exit(130);
			}
			if (jobs->mshell->status == 999)
				continue ;
			*/
			len1 = ft_strlen(buffer);
			len2 = ft_strlen(job->redir->eof[i]);
			if (buffer && !job->redir->eof[i + 1] && state
					&& !(!ft_strncmp(buffer, job->redir->eof[i], len1) && len1 == len2))
				ft_putendl_fd(buffer, pipe_fd[1]);
			if (!ft_strncmp(buffer, job->redir->eof[i], len1) && len1 == len2)
				i++;
			free(buffer);
		}
        exit(0);
    }
    close(pipe_fd[1]);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
    waitpid(job->pid, &temp_status, 0);
	if (WIFEXITED(temp_status))
		jobs->mshell->status = WEXITSTATUS(temp_status);
	if (jobs->mshell->status == 130)
		return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

/*
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
    int heredoc_pipe[2];
    // Heredoc işlemi için bir pipe açıp yazılacak içerikleri oraya göndereceğiz
    if (pipe(heredoc_pipe) == -1)
    {
        perror("pipe");
        return;
    }
    if (redir->eof)
        redir->args = heredoc(redir->eof);
    //write(redir->out_file, redir->args, ft_strlen(redir->args));
}
*/

static char	create_file_rd(char **files, int len)
{
	int	fd;
	int	i;
	if(files)
	{
		i = 0;
		while (i < len && files[i])
		{
			if (access(files[i], F_OK))
			{
				perror(files[i]);
				return (EXIT_FAILURE);
			}
			fd = open(files[i], O_RDONLY , 0644);
			/*
			if (fd == -1)
				continue ;//error
			*/
			/*
			if (fd == -1 && (jobs->len != 1 || job->is_builtin == false))
				error1;
			else if (fd == -1 && jobs->len == 1)
				return (error2);
			*/
			close(fd);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}

/*
static void	create_file_rd_append(char **files, int len)
{
	int	fd;
	int	i;

	i = 0;
	while (i < len && files[i])
	{
		fd = open(files[i], O_CREAT | O_RDONLY |O_APPEND);
		if (fd == -1)
			continue ;
		close(fd);
		i++;
	}
}
*/

//static char	create_file_wr(t_jobs *jobs, char **files, int len) // status
static char	create_file_wr(char **files, int len)
{
	int	fd;
	int	i;

	if(files)
	{
		i = 0;
		while (i < len && files[i])
		{
			if (access(files[i], F_OK))
			{
				perror(files[i]);
				return (EXIT_FAILURE);
			}
			fd = open(files[i], O_CREAT | O_WRONLY | O_TRUNC , 0644);
			if (fd == -1)
				continue ;
			close(fd);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}

static char	create_file_wr_append(char **files, int len)
{
	int	fd;
	int	i;
	if(files)
	{
		i = 0;
		while (i < len && files[i])
		{
			if (access(files[i], F_OK))
			{
				perror(files[i]);
				return (EXIT_FAILURE);
			}
			fd = open(files[i], O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (fd == -1)
				continue ;
			close(fd);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}

static char	set_input(t_job *job)
{
	int	len;

	if (job->redir->in_files)
	{
		len = str_arr_len(job->redir->in_files);
		if (create_file_rd(job->redir->in_files, len))
			return (EXIT_FAILURE);
		if (job->redir->last_in == IN)
		{
			if (access(job->redir->in_files[len - 1], F_OK))
			{
				perror(job->redir->in_files[len - 1]);
				return (EXIT_FAILURE);
			}
			job->redir->in_file = open(job->redir->in_files[len - 1], O_RDONLY, 0644);
			dup2(job->redir->in_file, 0);
			close(job->redir->in_file);
		}
	}
	return (EXIT_SUCCESS);
}

static char	set_output(t_job *job, int pipe_fd[2])
{
	int	len;
	int	fd;

	close(pipe_fd[0]);
	if (job->next_job && !job->redir->out_files && !job->redir->appends)
		dup2(pipe_fd[1], 1);
	else if (job->redir->out_files || job->redir->appends)
	{
		fd = 1;
		if (job->redir->last_out == APPEND)
		{
			if (create_file_wr(job->redir->out_files, str_arr_len(job->redir->out_files)))
				return (EXIT_FAILURE);
			len = str_arr_len(job->redir->appends);
			if (create_file_wr_append(job->redir->appends, len))
				return (EXIT_FAILURE);
			if (access(job->redir->appends[len - 1], F_OK))
			{
				perror(job->redir->appends[len - 1]);
				return (EXIT_FAILURE);
			}
			job->redir->append_file = open(job->redir->appends[len - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
			/*
			if (job->redir->append_file == -1 && (jobs->len != 1 || job->is_builtin == false))
				error1;
			else if (job->redir->append_file == -1 && jobs->len == 1)
				return (error2);
			*/
			fd = job->redir->append_file;
		}
		else if (job->redir->last_out == OUT)
		{
			if (create_file_wr_append(job->redir->appends, str_arr_len(job->redir->appends)))
				return (EXIT_FAILURE);
			len = str_arr_len(job->redir->out_files);
			if (create_file_wr(job->redir->out_files, len))
				return (EXIT_FAILURE);
			if (access(job->redir->out_files[len - 1], F_OK))
			{
				perror(job->redir->out_files[len - 1]);
				return (EXIT_FAILURE);
			}
			job->redir->out_file = open(job->redir->out_files[len - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			/*
			if (job->redir->out_file == -1 && (jobs->len != 1 || job->is_builtin == false))
				error1;
			else if (job->redir->out_file == -1 && jobs->len == 1)
				return (error2);
			*/
			fd = job->redir->out_file;
		}
		dup2(fd, 1);
		close(fd);
	}
	return (EXIT_SUCCESS);
}

static char	child_process(t_jobs *jobs, t_job *job, char *exec_path)
{
	int		len;
	int 	fd;

	fd = 1;
	if (job->redir->out_files || job->redir->appends)
	{
		if (job->redir->appends)
		{
			len = str_arr_len(job->redir->appends);
			if (create_file_wr_append(job->redir->appends, len))
				return (EXIT_FAILURE);
			if (access(job->redir->appends[len - 1], F_OK))
			{
				perror(job->redir->appends[len - 1]);
				return (EXIT_FAILURE);
			}
			job->redir->append_file = open(job->redir->appends[len - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
			fd = job->redir->append_file;
			if (create_file_wr(job->redir->out_files, str_arr_len(job->redir->out_files)))
				return (EXIT_FAILURE);
		}
		else if(job->redir->out_files)
		{
			len = str_arr_len(job->redir->out_files);
			if (create_file_wr(job->redir->out_files, len))
				return (EXIT_FAILURE);
			if (access(job->redir->out_files[len - 1], F_OK))
			{
				perror(job->redir->out_files[len - 1]);
				return (EXIT_FAILURE);
			}
			job->redir->out_file = open(job->redir->out_files[len - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			fd = job->redir->out_file;
			if (create_file_wr_append(job->redir->appends, str_arr_len(job->redir->appends)))
				return (EXIT_FAILURE);
		}
		dup2(fd, 1);
		close(fd);
	}
	if (job->redir->in_files)
		set_input(job);
	execve(exec_path, job->args, env_to_double_pointer(jobs->env));
	perror("exec");
	return (EXIT_FAILURE);
}

static char	no_pipe(t_jobs *jobs, t_job *job, char *exec_path)
{
	int	len;
	int	fd;

	is_builtin(job);
	if (job->is_builtin == false)
	{
		job->pid = fork();
		if (job->pid == 0)
		{
			//set_signal(CHILD_P);
			child_process(jobs, job, exec_path);
			exit(jobs->mshell->status);
		}
	}
	if (job->redir->out_files || job->redir->appends)
	{
		if (job->is_builtin == false)
			fd = 0;
		if (job->redir->appends && job->is_builtin == true)
		{
			len = str_arr_len(job->redir->appends);
			if (create_file_wr_append(job->redir->appends, len))
				return (EXIT_FAILURE);
			if (access(job->redir->appends[len - 1], F_OK))
			{
				perror(job->redir->appends[len - 1]);
				return (EXIT_FAILURE);
			}
			job->redir->append_file = open(job->redir->appends[len - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
			fd = job->redir->append_file;
			if (create_file_wr(job->redir->out_files, str_arr_len(job->redir->out_files)))
				return (EXIT_FAILURE);
		}
		else if (job->redir->out_files && job->is_builtin == true)
		{
			len = str_arr_len(job->redir->out_files);
			if (create_file_wr(job->redir->out_files, len))
				return (EXIT_FAILURE);
			if (access(job->redir->out_files[len - 1], F_OK))
			{
				perror(job->redir->out_files[len - 1]);
				return (EXIT_FAILURE);
			}
			job->redir->out_file = open(job->redir->out_files[len - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			fd = job->redir->out_file;
			if (create_file_wr_append(job->redir->appends, str_arr_len(job->redir->appends)))
				return (EXIT_FAILURE);
		}
		dup2(fd, 1);
		close(fd);
		if (fd == -1)
		{
			jobs->mshell->status = 1;
			return (EXIT_SUCCESS);
		}
	}
	if (job->is_builtin == true)
	{
		if (job->redir->in_files)
		{
			set_input(job);
			if (job->redir->in_file == -1)
			{
				jobs->mshell->status = 1;
				return (EXIT_SUCCESS);
			}
		}
		return (ctrl_builtins(jobs, job));
	}
	return (EXIT_SUCCESS);
}

static void pipe_handle(t_jobs *jobs, t_job *job, char *exec_path)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(127); // \?
	}
	job->pid = fork();
	if (job->pid == 0)
	{
		//set_signal(CHILD_P);
		close(pipe_fd[0]);
		set_input(job);
		set_output(job, pipe_fd);
		close(pipe_fd[1]);
		is_builtin(job);
		if (job->is_builtin == false)
		{
			execve(exec_path, job->args, env_to_char_array(jobs->env));
			perror("exec");
		}
		ctrl_builtins(jobs, job);
		exit(127);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
}

char	executor(t_mshell *mshell)
{
	t_job	*temp_job;
	char	*exec_path;
	int		temp_status;
	int		i;

	mshell->backup_fd[0] = dup(STDIN_FILENO);
	mshell->backup_fd[1] = dup(STDOUT_FILENO);
	temp_job = mshell->jobs->job_list;
	while (temp_job)
	{
		exec_path = find_path(get_env_value(mshell->jobs->env, "PATH"), temp_job->cmd);
		if (mshell->jobs->len == 1)
		{
			if (temp_job->redir->eof && heredoc(mshell->jobs, temp_job, 1))
				break ;
			//if (no_pipe(mshell->jobs, temp_job, exec_path))
			//	return (EXIT_FAILURE);
			no_pipe(mshell->jobs, temp_job, exec_path);
		}
		else
		{
			if (temp_job->redir->eof && heredoc(mshell->jobs, temp_job, 0))
				break ;
			pipe_handle(mshell->jobs, temp_job, exec_path);
		}
		temp_job = temp_job->next_job;
	}
	dup2(mshell->backup_fd[0], 0);
	close(mshell->backup_fd[0]);
	dup2(mshell->backup_fd[1], 1);
	close(mshell->backup_fd[1]);
	temp_status = 0;
	temp_job = mshell->jobs->job_list;
	if (mshell->jobs->len == 1 && temp_job->is_builtin == true)
		return (EXIT_SUCCESS);
	while (temp_job)
	{
		signal_handle_exec(mshell);
		i = waitpid(temp_job->pid, &temp_status, 0);
		if (i < 0)
			continue ;
		is_builtin(temp_job);
		if (mshell->jobs->len == 1 && temp_job->is_builtin == true)
			break ;
		if (WIFEXITED(temp_status))
			mshell->status = WEXITSTATUS(temp_status);
		else if (WIFSIGNALED(temp_status))
			mshell->status = 128 + WTERMSIG(temp_status);
		temp_job = temp_job->next_job;
	}
	return (EXIT_SUCCESS);
}

/*
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
    handle_redirects(temp_job->redir);
    if (ctrl_builtins(jobs, temp_job) == -1)
    {
        temp = find_path(get_env_value(jobs->env, "PATH"), temp_job->cmd);
        if (!temp)
        {
            printf("exec error\n");
            return ;
        }
        execve(temp, temp_job->args, envp);
        perror("execve");
    }
}

char executor(t_mshell *mshell)
{
    pid_t   pid;
    char    state;
    int     i;

    if (mshell->jobs->len == 1)
    {
        state = ctrl_builtins(mshell->jobs, mshell->jobs->job_list);
        if (state == -1)
            exec_child(0, mshell->jobs, env_to_double_pointer(mshell->jobs->env));
        else if (state == EXIT_SUCCESS)
            return (EXIT_SUCCESS);
    }
    i = -1;
    while (++i < mshell->jobs->len)
    {
        if (i + 1 < mshell->jobs->len)
        {
            if (pipe(mshell->jobs->active_pipe) == -1)
                return (perror("pipe"), EXIT_FAILURE);
        }
        pid = fork();
        if (pid == -1)
            return (perror("fork"), EXIT_FAILURE);
        if (pid == 0)
            exec_child(i, mshell->jobs, env_to_double_pointer(mshell->jobs->env));
        if (i > 0)
            close_pipe(mshell->jobs->old_pipe);
        if (i + 1 < mshell->jobs->len)
        {
            mshell->jobs->old_pipe[0] = mshell->jobs->active_pipe[0];
            mshell->jobs->old_pipe[1] = mshell->jobs->active_pipe[1];
        }
        waitpid(pid, &mshell->status, 0);
    }
    return (EXIT_SUCCESS);
}


static void	pipe_heredoc(t_jobs *jobs, t_job *job, int fd[2], char **exec_str_arr)
{
	int	pipe[2];
	int	status;

	if (pipe(pipe) == -1)
	{
		perror("pipe");
		exit(127);
	}
	job->pid = fork();
	if (job->pid == 0)
	{
		//set_signal(HEREDOC_P);
		//pheredoc(fd, job, pipe);
	}
	close(pipe[1]);
	dup2(p[0], 0);
	close(pipe[0]);
	waitpid(job->pid, &jobs->mshell->status, 0);
	//if (WIFEXITED(status))
	//	jobs->mshell->status = WEXITSTATUS(status);
	//if (jobs->mshell->status == 130)
	//	freecpointer(run);
}
*/
