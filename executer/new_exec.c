#include "../minishell.h"

void	free_str_arr(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i])
	{
		free(str_arr[i]);
		i++;
	}
	free(str_arr);
}

int str_arr_len(char **str_arr)
{
	int	i;

	if (!str_arr || !*str_arr)
		return (0);
	i = 0;
	while (str_arr[i])
		i++;
	return (i);
}

char **str_arr_realloc(char **str_arr, char *element)
{
	char **rtrn;
	int	i;

	if (!str_arr)
	{
		rtrn = ft_calloc(2, sizeof(char *));
		rtrn[0] = ft_strdup(element);
		free(element);
		return (rtrn);
	}
	i = 0;
	while (str_arr[i])
		i++;
	rtrn = ft_calloc(i + 2, sizeof(char *));
	i = -1;
	while (str_arr[++i])
	{
		rtrn[i] = ft_strdup(str_arr[i]);
		free(str_arr[i]);
	}
	free(str_arr);
	rtrn[i] = ft_strdup(element);
	rtrn[i + 1] = NULL;
	free(element);
	return (rtrn);
}

char	*find_path(char *path, char *cmd)
{
	char	**splitted_path;
	char	*temp;
	char	*temp2;
	int		i;

	splitted_path = ft_split(path, ':');
	i = 0;
	while (splitted_path[i])
	{
		temp = ft_strjoin(splitted_path[i], "/");
		temp2 = ft_strjoin(temp, cmd);
		if (!access(temp2, X_OK))
			return ((free_str_arr(splitted_path)), temp2);
		free(temp);
		free(temp2);
		i++;
	}
	return (NULL);
}

void	free_redir(t_redir *redir)
{
	if (redir->appends)
	{
		free_str_arr(redir->appends);
		redir->appends = NULL;
	}
	if (redir->eof)
	{
		free_str_arr(redir->eof);
		redir->eof = NULL;
	}
	if (redir->in_files)
	{
		free_str_arr(redir->in_files);
		redir->in_files = NULL;
	}
	if (redir->out_files)
	{
		free_str_arr(redir->out_files);
		redir->out_files = NULL;
	}
	free(redir);
}

void	free_job(t_job *job)
{
	if (job->args)
	{
		free_str_arr(job->args);
		job->args = NULL;
	}
	if (job->redir)
	{
		free_redir(job->redir);
		job->redir = NULL;
	}
	free(job);
}
