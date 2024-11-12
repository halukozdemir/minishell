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
			set_signal(HDOC_SF);
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
			if (!buffer)
			{
				exit_status = 130;
				exit(exit_status);
			}
			/*
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
		exit_status = WEXITSTATUS(temp_status);
	if (exit_status == 130)
		return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

static char	redir_error(t_jobs *jobs, t_job *job, char *file_i, int fd)
{
	t_stat	stat_t;
	int		state;

	if (fd == -1 && (jobs->len != 1 || job->is_builtin == false))
	{
		exit_status = 1;
		write(2, "minishell: ", 12);
		write(2, file_i, ft_strlen(file_i));
		state = stat(file_i, &stat_t);
		if (!state)
			S_ISDIR(stat_t.st_mode);
		else
		{
			write(2, ": Is a directory\n", 18);
			exit(1);
		}
		if (!state)
			S_ISREG(stat_t.st_mode);
		else
		{
			write(2, ": No such file or directory\n", 29);
			exit(1);
		}
	}
	else if (fd == -1 && jobs->len == 1)
	{
		exit_status = 1;
		write(2, "minishell: ", 12);
		write(2, file_i, ft_strlen(file_i));
		state = stat(file_i, &stat_t);
		if (!state)
			S_ISDIR(stat_t.st_mode);
		else
			write(2, ": Is a directory\n", 18);
		if (!state)
			S_ISREG(stat_t.st_mode);
		else
			write(2, ": No such file or directory\n", 29);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static char	create_file_rd(t_jobs *jobs,t_job *job, char **files, int len)
{
	int		fd;
	int		i;

	if(files)
	{
		i = 0;
		while (i < len && files[i])
		{
			if (access(files[i], F_OK))
			{
				write(2, "minishell: ", 12);
				write(2, files[i], ft_strlen(files[i]));
				write(2, ": No such file or directory\n", 29);
				return (EXIT_FAILURE);
			}
			if (access(files[i], R_OK))
			{
				write(2, "minishell: ", 12);
				write(2, files[i], ft_strlen(files[i]));
				write(2, ": Permission denied\n", 21);
				exit(1);
			}
			fd = open(files[i], O_RDONLY, 0644);
			if (redir_error(jobs, job, files[i], fd))
				return (EXIT_FAILURE);
			close(fd);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}

static char	create_file_wr(t_jobs *jobs,t_job *job, char **files, int len)
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
				write(2, "minishell: ", 12);
				write(2, files[i], ft_strlen(files[i]));
				write(2, ": No such file or directory\n", 29);
				return (EXIT_FAILURE);
			}
			if (access(files[i], R_OK))
			{
				write(2, "minishell: ", 12);
				write(2, files[i], ft_strlen(files[i]));
				write(2, ": Permission denied\n", 21);
				exit(1);
			}
			fd = open(files[i], O_CREAT | O_WRONLY | O_TRUNC , 0644);
			if (redir_error(jobs, job, files[i], fd))
				return (EXIT_FAILURE);
			close(fd);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}

static char	create_file_wr_append(t_jobs *jobs,t_job *job, char **files, int len)
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
				write(2, "minishell: ", 12);
				write(2, files[i], ft_strlen(files[i]));
				write(2, ": No such file or directory\n", 29);
				return (EXIT_FAILURE);
			}
			if (access(files[i], R_OK))
			{
				write(2, "minishell: ", 12);
				write(2, files[i], ft_strlen(files[i]));
				write(2, ": Permission denied\n", 21);
				exit(1);
			}
			fd = open(files[i], O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (redir_error(jobs, job, files[i], fd))
				return (EXIT_FAILURE);
			close(fd);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}

static char	set_input(t_jobs *jobs, t_job *job)
{
	int	len;

	if (job->redir->in_files)
	{
		len = str_arr_len(job->redir->in_files);
		if (create_file_rd(jobs, job, job->redir->in_files, len))
			return (EXIT_FAILURE);
		if (job->redir->last_in == IN)
		{
			if (access(job->redir->in_files[len - 1], F_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->in_files[len - 1], ft_strlen(job->redir->in_files[len - 1]));
				write(2, ": No such file or directory\n", 29);
				return (EXIT_FAILURE);
			}
			if (access(job->redir->in_files[len - 1], R_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->in_files[len - 1], ft_strlen(job->redir->in_files[len - 1]));
				write(2, ": Permission denied\n", 21);
				return (EXIT_FAILURE);
			}
			job->redir->in_file = open(job->redir->in_files[len - 1], O_RDONLY, 0644);
			dup2(job->redir->in_file, 0);
			close(job->redir->in_file);
		}
	}
	return (EXIT_SUCCESS);
}

static char	set_output(t_jobs *jobs, t_job *job, int pipe_fd[2])
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
			if (create_file_wr(jobs, job, job->redir->out_files, str_arr_len(job->redir->out_files)))
				return (EXIT_FAILURE);
			len = str_arr_len(job->redir->appends);
			if (create_file_wr_append(jobs, job, job->redir->appends, len))
				return (EXIT_FAILURE);
			if (access(job->redir->appends[len - 1], F_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->appends[len - 1], ft_strlen(job->redir->appends[len - 1]));
				write(2, ": No such file or directory\n", 29);
				return (EXIT_FAILURE);
			}
			if (access(job->redir->appends[len - 1], R_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->appends[len - 1], ft_strlen(job->redir->appends[len - 1]));
				write(2, ": Permission denied\n", 21);
				return (EXIT_FAILURE);
			}
			job->redir->append_file = open(job->redir->appends[len - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
			fd = job->redir->append_file;
			if (redir_error(jobs, job, job->redir->appends[len - 1], fd))
				return (EXIT_FAILURE);
		}
		else if (job->redir->last_out == OUT)
		{
			if (create_file_wr_append(jobs, job, job->redir->appends, str_arr_len(job->redir->appends)))
				return (EXIT_FAILURE);
			len = str_arr_len(job->redir->out_files);
			if (create_file_wr(jobs, job, job->redir->out_files, len))
				return (EXIT_FAILURE);
			if (access(job->redir->out_files[len - 1], F_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->out_files[len - 1], ft_strlen(job->redir->out_files[len - 1]));
				write(2, ": No such file or directory\n", 29);
				return (EXIT_FAILURE);
			}
			if (access(job->redir->out_files[len - 1], R_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->out_files[len - 1], ft_strlen(job->redir->out_files[len - 1]));
				write(2, ": Permission denied\n", 21);
				return (EXIT_FAILURE);
			}
			job->redir->out_file = open(job->redir->out_files[len - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			fd = job->redir->out_file;
			if (redir_error(jobs, job, job->redir->out_files[len - 1], fd))
				return (EXIT_FAILURE);
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
			if (create_file_wr_append(jobs, job, job->redir->appends, len))
				return (EXIT_FAILURE);
			if (access(job->redir->appends[len - 1], F_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->appends[len - 1], ft_strlen(job->redir->appends[len - 1]));
				write(2, ": No such file or directory\n", 29);
				return (EXIT_FAILURE);
			}
			if (access(job->redir->appends[len - 1], R_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->appends[len - 1], ft_strlen(job->redir->appends[len - 1]));
				write(2, ": Permission denied\n", 21);
				return (EXIT_FAILURE);
			}
			job->redir->append_file = open(job->redir->appends[len - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
			fd = job->redir->append_file;
			if (create_file_wr(jobs, job, job->redir->out_files, str_arr_len(job->redir->out_files)))
				return (EXIT_FAILURE);
		}
		else if(job->redir->out_files)
		{
			len = str_arr_len(job->redir->out_files);
			if (create_file_wr(jobs, job, job->redir->out_files, len))
				return (EXIT_FAILURE);
			if (access(job->redir->out_files[len - 1], F_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->out_files[len - 1], ft_strlen(job->redir->out_files[len - 1]));
				write(2, ": No such file or directory\n", 29);
				return (EXIT_FAILURE);
			}
			if (access(job->redir->out_files[len - 1], R_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->out_files[len - 1], ft_strlen(job->redir->out_files[len - 1]));
				write(2, ": Permission denied\n", 21);
				return (EXIT_FAILURE);
			}
			job->redir->out_file = open(job->redir->out_files[len - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			fd = job->redir->out_file;
			if (create_file_wr_append(jobs, job, job->redir->appends, str_arr_len(job->redir->appends)))
				return (EXIT_FAILURE);
		}
		dup2(fd, 1);
		close(fd);
	}
	if (job->redir->in_files && set_input(jobs, job))
		return (EXIT_FAILURE);
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
			set_signal(CHILD_SF);
			if (child_process(jobs, job, exec_path))
				return (EXIT_FAILURE);
			exit(exit_status);
		}
	}
	if (job->redir->out_files || job->redir->appends)
	{
		if (job->is_builtin == false)
			fd = 0;
		if (job->redir->appends && job->is_builtin == true)
		{
			len = str_arr_len(job->redir->appends);
			if (create_file_wr_append(jobs, job, job->redir->appends, len))
				return (EXIT_FAILURE);
			if (access(job->redir->appends[len - 1], F_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->appends[len - 1], ft_strlen(job->redir->appends[len - 1]));
				write(2, ": No such file or directory\n", 29);
				return (EXIT_FAILURE);
			}
			if (access(job->redir->appends[len - 1], R_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->appends[len - 1], ft_strlen(job->redir->appends[len - 1]));
				write(2, ": Permission denied\n", 21);
				return (EXIT_FAILURE);
			}
			job->redir->append_file = open(job->redir->appends[len - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
			fd = job->redir->append_file;
			if (create_file_wr(jobs, job, job->redir->out_files, str_arr_len(job->redir->out_files)))
				return (EXIT_FAILURE);
		}
		else if (job->redir->out_files && job->is_builtin == true)
		{
			len = str_arr_len(job->redir->out_files);
			if (create_file_wr(jobs, job, job->redir->out_files, len))
				return (EXIT_FAILURE);
			if (access(job->redir->out_files[len - 1], F_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->out_files[len - 1], ft_strlen(job->redir->out_files[len - 1]));
				write(2, ": No such file or directory\n", 29);
				return (EXIT_FAILURE);
			}
			if (access(job->redir->out_files[len - 1], R_OK))
			{
				write(2, "minishell: ", 12);
				write(2, job->redir->out_files[len - 1], ft_strlen(job->redir->out_files[len - 1]));
				write(2, ": Permission denied\n", 21);
				return (EXIT_FAILURE);
			}
			job->redir->out_file = open(job->redir->out_files[len - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			fd = job->redir->out_file;
			if (create_file_wr_append(jobs, job, job->redir->appends, str_arr_len(job->redir->appends)))
				return (EXIT_FAILURE);
		}
		dup2(fd, 1);
		close(fd);
		if (fd == -1)
		{
			exit_status = 1;
			return (EXIT_FAILURE);
		}
	}
	if (job->is_builtin == true)
	{
		if (job->redir->in_files)
		{
			if (set_input(jobs, job))
				return (EXIT_FAILURE);
			if (job->redir->in_file == -1)
			{
				exit_status = 1;
				return (EXIT_FAILURE);
			}
		}
		return (ctrl_builtins(jobs, job));
	}
	return (EXIT_SUCCESS);
}

static char pipe_handle(t_jobs *jobs, t_job *job, char *exec_path)
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
		set_signal(CHILD_SF);
		close(pipe_fd[0]);
		if (set_input(jobs, job))
			return (EXIT_FAILURE);
		if (set_output(jobs, job, pipe_fd))
			return (EXIT_FAILURE);
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
	return (EXIT_SUCCESS);
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
			if (no_pipe(mshell->jobs, temp_job, exec_path))
				return (EXIT_SUCCESS);
		}
		else
		{
			if (temp_job->redir->eof && heredoc(mshell->jobs, temp_job, 0))
				break ;
			if (exit_status == 130)
				break ;
			if (pipe_handle(mshell->jobs, temp_job, exec_path))
				return (EXIT_SUCCESS);
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
		signal(SIGINT, &handler_sigint);
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
