#include "../minishell.h"

void	is_builtin(t_job *job)
{
	if (!ft_strncmp(job->args[0], "pwd", 3)
			&& ft_strlen(job->args[0]) == 3)
		job->is_builtin = true;
	else if (!ft_strncmp(job->args[0], "cd", 2) && ft_strlen(job->args[0]) == 2)
		job->is_builtin = true;
	else if (!ft_strncmp(job->args[0], "echo", 4) && ft_strlen(job->args[0]) == 4)
		job->is_builtin = true;
	else if (!ft_strncmp(job->args[0], "env", 3) && ft_strlen(job->args[0]) == 3)
		job->is_builtin = true;
	else if (!ft_strncmp(job->args[0], "exit", 4) && ft_strlen(job->args[0]) == 4)
		job->is_builtin = true;
	else if (!ft_strncmp(job->args[0], "unset", 5) && ft_strlen(job->args[0]) == 5)
		job->is_builtin = true;
	else if (!ft_strncmp(job->args[0], "export", 6) && ft_strlen(job->args[0]) == 6)
		job->is_builtin = true;
	else
		job->is_builtin = false;
}

char	ctrl_builtins(t_jobs *jobs, t_job *job)
{
	char	state;

	if (!ft_strncmp(job->args[0], "pwd", 3)
			&& ft_strlen(job->args[0]) == 3)
	{
		state = pwd();
		return (state);
	}
	else if (!ft_strncmp(job->args[0], "cd", 2) && ft_strlen(job->args[0]) == 2)
	{
		state = cd(jobs->mshell, job->args[1]);
		return (state);
	}
	else if (!ft_strncmp(job->args[0], "echo", 4) && ft_strlen(job->args[0]) == 4)
	{
		echo(job);
		return (EXIT_SUCCESS);
	}
	else if (!ft_strncmp(job->args[0], "env", 3) && ft_strlen(job->args[0]) == 3)
	{
		env(jobs->env);
		return (EXIT_SUCCESS);
	}
	else if (!ft_strncmp(job->args[0], "exit", 4) && ft_strlen(job->args[0]) == 4)
	{
		exit_d(jobs->mshell, job->args);
		return (EXIT_SUCCESS);
	}
	else if (!ft_strncmp(job->args[0], "unset", 5) && ft_strlen(job->args[0]) == 5)
	{
		unset(jobs->env, job->args);
		return (EXIT_SUCCESS);
	}
	else if (!ft_strncmp(job->args[0], "export", 6) && ft_strlen(job->args[0]) == 6)
	{
		state = export(jobs->env, jobs->mshell, job->args);
		return (state);
	}
	return (-1);
}
