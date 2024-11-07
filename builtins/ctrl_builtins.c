#include "../minishell.h"

char	ctrl_builtins(t_jobs *jobs, t_job *job)
{
	if (!ft_strncmp(job->args[0], "pwd", 3)
			&& ft_strlen(job->args[0]) == 3)
		return (pwd());
	else if (!ft_strncmp(job->args[0], "cd", 2) && ft_strlen(job->args[0]) == 2)
		return (cd(job->args[1]));
	else if (!ft_strncmp(job->args[0], "echo", 4) && ft_strlen(job->args[0]) == 4)
		return (echo(job->args), EXIT_SUCCESS);
	else if (!ft_strncmp(job->args[0], "env", 3) && ft_strlen(job->args[0]) == 3)
		return (env(jobs->env), EXIT_SUCCESS);
	else if (!ft_strncmp(job->args[0], "exit", 4) && ft_strlen(job->args[0]) == 4)
		return (exit_d(job->args), EXIT_SUCCESS);
	//else if (!ft_strncmp(job->args[0], "unset", 5) && ft_strlen(job->args[0]) == 5)
	//	return (unset(&jobs->env, ));
	else if (!ft_strncmp(job->args[0], "export", 6) && ft_strlen(job->args[0]) == 6)
		return (export(jobs->env, jobs->mshell, job->args[1]));
	return (-1);
}
