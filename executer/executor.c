/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 14:03:53 by halozdem          #+#    #+#             */
/*   Updated: 2024/11/18 14:03:53 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	access_error(char *file, const char *message)
{
	g_exit_status = 1;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd((char *) message, 2);
}

static char **free_env_array(char **env_array, int count)
{
    while (--count >= 0)
        free(env_array[count]);
    free(env_array);
    return (NULL);
}

static int get_env_count(t_env *env_list)
{
    int count = 0;
    t_env *temp = env_list;
    while (temp)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

static char **env_to_double_pointer(t_env *env_list)
{
    t_env   *temp;
    char    **env_array;
    char    *temp_str;
    int     count;
    int     i;

    count = get_env_count(env_list);
    env_array = malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);
    temp = env_list;
    i = 0;
    while (i < count)
    {
        temp_str = ft_strjoin(temp->key, "=");
        if (!temp_str)
            return (free_env_array(env_array, i));
        env_array[i] = ft_strjoin(temp_str, temp->value);
        free(temp_str);
        if (!env_array[i])
            return (free_env_array(env_array, i));
        i++;
        temp = temp->next;
    }
    return ((env_array[count] = NULL), env_array);
}

static void errL(char *path)
{
	t_stat	stat_t;

	write(2, "minishell: ", 12);
	write(2, path, ft_strlen(path));
	stat(path, &stat_t);
	if (S_ISDIR(stat_t.st_mode))
	{
		g_exit_status = 1;
		write(2, ": Is a directory\n", 18);
		exit(126);
	}
	else if (!S_ISREG(stat_t.st_mode))
	{
		g_exit_status = 1;
		write(2, ": No such file or directory\n", 29);
		exit(127);
	}
	if (access(path, X_OK))
	{
		g_exit_status = 1;
		ft_putendl_fd(":  Permission denied", 2);
		exit(126);
	}
}

static void handle_no_env_path(t_jobs *jobs, t_job *job)
{
    if (!access(job->args[0], X_OK))
    {
        execve(job->args[0], job->args, env_to_double_pointer(jobs->env));
        exit(127);
    }
    else
    {
        g_exit_status = 1;
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(job->args[0], 2);
        ft_putendl_fd(": No such file or directory", 2);
    }
}

static void handle_exec_path_error(t_job *job)
{
    g_exit_status = 1;
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(job->args[0], 2);
    ft_putendl_fd(": command not found", 2);
    exit(127);
}

static void runCmd(t_jobs *jobs, t_job *job)
{
    char *env_path;
    char *exec_path;

    env_path = get_env_value(jobs->env, "PATH");
    if (!env_path)
    {
        handle_no_env_path(jobs, job);
        return;
    }
    if (ft_strchr(job->args[0], '/'))
    {
        errL(job->args[0]);
        exec_path = job->args[0];
    }
    else
        exec_path = find_path(env_path, job->args[0]);
    if (!exec_path)
        handle_exec_path_error(job);
    g_exit_status = 0;
    execve(exec_path, job->args, env_to_double_pointer(jobs->env));
    exit(127);
}

void	handle_eof_condition(t_job *job, int *i, char *buffer, int *pipe_fd)
{
	int	len1;
	int	len2;

	len1 = ft_strlen(buffer);
	len2 = ft_strlen(job->redir->eof[*i]);
	if (!job->redir->eof[*i + 1] && buffer && !(!ft_strncmp(buffer, job->redir->eof[*i], len1) && len1 == len2))
		ft_putendl_fd(buffer, pipe_fd[1]);
	if (!ft_strncmp(buffer, job->redir->eof[*i], len1) && len1 == len2)
		(*i)++;
	free(buffer);
}

void	child_process(t_jobs *jobs, t_job *job, char state, int *pipe_fd)
{
	char	*buffer;
	int		i;

	if (state)
		set_signal(HDOC_SF);
	dup2(jobs->mshell->backup_fd[0], 0);
	i = 0;
	while (job->redir->eof[i])
	{
		buffer = readline(">");
		if (!buffer)
		{
			g_exit_status = 130;
			exit(g_exit_status);
		}
		handle_eof_condition(job, &i, buffer, pipe_fd);
	}
	exit(0);
}

void	wait_for_child(pid_t pid, int *temp_status, char state)
{
	waitpid(pid, temp_status, 0);
	if (state && WIFEXITED(*temp_status))
		g_exit_status = WEXITSTATUS(*temp_status);
}

char	heredoc(t_jobs *jobs, t_job *job, char state)
{
	int		temp_status;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (EXIT_FAILURE);
	job->pid = fork();
	if (job->pid == 0)
	{
		child_process(jobs, job, state, pipe_fd);
	}
	wait_for_child(job->pid, &temp_status, state);
	if (g_exit_status == 130)
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
}
static void	print_error_message(char *file_i, char *message)
{
	write(2, "minishell: ", 12);
	write(2, file_i, ft_strlen(file_i));
	write(2, message, ft_strlen(message));
}

static void	handle_stat_error(char *file_i)
{
	t_stat	stat_t;

	stat(file_i, &stat_t);
	if (S_ISDIR(stat_t.st_mode))
	{
		print_error_message(file_i, ": Is a directory\n");
		exit(1);
	}
	else
	{
		print_error_message(file_i, ": No such file or directory\n");
		exit(1);
	}
}

static char	redir_error(t_jobs *jobs, t_job *job, char *file_i, int fd)
{
	if (fd == -1)
	{
		g_exit_status = 1;
		if (jobs->len != 1 || job->is_builtin == false)
			handle_stat_error(file_i);
		else
		{
			t_stat	stat_t;
			stat(file_i, &stat_t);
			if (S_ISDIR(stat_t.st_mode))
				print_error_message(file_i, ": Is a directory\n");
			else
				print_error_message(file_i, ": No such file or directory\n");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

static char	file_control(t_jobs *jobs, t_job *job, char *file, int fd)
{
	if (!file)
		return (EXIT_FAILURE);
	if (access(file, F_OK))
	{
		access_error(file, ": No such file or directory\n");
		return (EXIT_FAILURE);
	}
	if (access(file, R_OK))
	{
		access_error(file, ": Permission denied\n");
		exit(1);
	}
	if (redir_error(jobs, job, file, fd))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	get_fd(t_jobs *jobs, t_job *job)
{
	char	state;
	int		len;
	int 	fd;
	int		i_out;
	int		i_app;
	int		i_in;
	int		i;

	fd = 1;
	i_out = 0;
	i_app = 0;
	i_in = 0;
	i = -1;
	state = 0;
	if (job->redir->files_order)
	{
		while (job->redir->files_order[++i])
		{
			len = ft_strlen(job->redir->files_order[i]);
			if (job->redir->out_files)
			{
				if (!ft_strncmp(job->redir->files_order[i], job->redir->out_files[i_out], len)
					&& len == (int) ft_strlen(job->redir->out_files[i_out]))
				{
					fd = open(job->redir->out_files[i_out], O_CREAT | O_WRONLY | O_TRUNC, 0644);
					if (file_control(jobs, job, job->redir->out_files[i_out], fd))
						return (-1);
					job->redir->out_file = fd;
					state = 1;
					i_out++;
				}
			}
			if (job->redir->appends)
			{
				if (!ft_strncmp(job->redir->files_order[i], job->redir->appends[i_app], len)
					&& len == (int) ft_strlen(job->redir->appends[i_app]))
				{
					fd = open(job->redir->appends[i_app], O_CREAT | O_WRONLY | O_APPEND, 0644);
					if (file_control(jobs, job, job->redir->appends[i_app], fd))
						return (-1);
					job->redir->append_file = fd;
					state = 1;
					i_app++;
				}
			}
			if (job->redir->in_files)
			{
				if (!ft_strncmp(job->redir->files_order[i], job->redir->in_files[i_in], len)
					&& len == (int) ft_strlen(job->redir->in_files[i_in]))
				{
					fd = open(job->redir->in_files[i_in], O_RDONLY, 0644);
					if (file_control(jobs, job, job->redir->in_files[i_in], fd))
						return (-1);
					job->redir->in_file = fd;
					state = 0;
					i_in++;
				}
			}
			if (state)
				dup2(fd, 1);
			else
				dup2(fd, 0);
			close(fd);
			if (fd == -1)
			{
				g_exit_status = 1;
				return (-1);
			}
		}
	}
	return (fd);
}

static char	no_pipe(t_jobs *jobs, t_job *job)
{
	int	fd;

	is_builtin(job);
	fd = get_fd(jobs, job);
	if (fd == -1)
		return (EXIT_FAILURE);
	if (job->is_builtin == false)
	{
		job->pid = fork();
		if (job->pid == 0)
		{
			set_signal(CHILD_SF);
			runCmd(jobs, job);
			exit(g_exit_status);
		}
	}
	else
		return (ctrl_builtins(jobs, job));
	return (EXIT_SUCCESS);
}

static char pipe_handle(t_jobs *jobs, t_job *job)
{
	int	pipe_fd[2];
	int	fd;

	fd = 1;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(127);
	}
	job->pid = fork();
	if (job->pid == 0)
	{
		set_signal(CHILD_SF);
		close(pipe_fd[0]);
		if (job->next_job)
		{
			dup2(pipe_fd[1], 1);
			close(pipe_fd[1]);
		}
		if (job->redir->in_files || job->redir->out_files || job->redir->appends)
		{
			fd = get_fd(jobs, job);
		}
		if (fd == -1)
			exit(1);
		is_builtin(job);
		if (job->is_builtin == false)
			runCmd(jobs, job);
		exit(ctrl_builtins(jobs, job));
	}
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (EXIT_SUCCESS);
}

void wait_child(t_mshell *mshell)
{
	t_job	*temp_job;
	int		temp_status;
	int		i;

	temp_job = mshell->jobs->job_list;
	if (mshell->jobs->len == 1 && temp_job->is_builtin == true)
		return ;
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
			g_exit_status = WEXITSTATUS(temp_status);
		else if (WIFSIGNALED(temp_status))
			g_exit_status = 128 + WTERMSIG(temp_status);
		temp_job = temp_job->next_job;
	}
}

char	executor(t_mshell *mshell)
{
	t_job	*temp_job;
	int		state;

	state = 1;
	mshell->backup_fd[0] = dup(STDIN_FILENO);
	mshell->backup_fd[1] = dup(STDOUT_FILENO);
	temp_job = mshell->jobs->job_list;
	while (temp_job)
	{
		if (mshell->jobs->len == 1)
		{
			if (temp_job->redir->eof && heredoc(mshell->jobs, temp_job, 1))
			{
				state = 0;
				break ;
			}
			if (no_pipe(mshell->jobs, temp_job))
				return (EXIT_SUCCESS);
		}
		else
		{
			if (temp_job->redir->eof && heredoc(mshell->jobs, temp_job, 0))
			{
				state = 0;
				break ;
			}
			if (g_exit_status == 130)
			{
				state = 0;
				break ;
			}
			if (pipe_handle(mshell->jobs, temp_job))
				return (EXIT_SUCCESS);
			g_exit_status = 0;
		}
		temp_job = temp_job->next_job;
	}
	if (state)
	{
		dup2(mshell->backup_fd[0], 0);
		close(mshell->backup_fd[0]);
		dup2(mshell->backup_fd[1], 1);
		close(mshell->backup_fd[1]);
		wait_child(mshell);
	}
	return (EXIT_SUCCESS);
}
