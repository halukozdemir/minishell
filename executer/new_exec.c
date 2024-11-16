#include "../minishell.h"

// void	free_str_arr(char **str_arr)
// {
// 	int	i;

// 	i = 0;
// 	while (str_arr[i])
// 	{
// 		free(str_arr[i]);
// 		i++;
// 	}
// 	free(str_arr);
// }

void free_str_arr(char **arr)
{
    int i = 0;

    if (!arr)
        return;

    while (arr[i])
    {
        free(arr[i]); // Her stringi serbest bırak
        i++;
    }
    free(arr); // String dizisinin kendisini serbest bırak
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
		return (rtrn);
	}
	i = 0;
	while (str_arr[i])
		i++;
	rtrn = ft_calloc(i + 2, sizeof(char *));
	i = -1;
	while (str_arr[++i])
		rtrn[i] = str_arr[i];
	rtrn[i] = ft_strdup(element);
	free(str_arr);
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

// void	free_redir(t_redir *redir)
// {
// 	if (redir->appends)
// 	{
// 		free_str_arr(redir->appends);
// 		redir->appends = NULL;
// 	}
// 	if (redir->eof)
// 	{
// 		free_str_arr(redir->eof);
// 		redir->eof = NULL;
// 	}
// 	if (redir->in_files)
// 	{
// 		free_str_arr(redir->in_files);
// 		redir->in_files = NULL;
// 	}
// 	if (redir->out_files)
// 	{
// 		free_str_arr(redir->out_files);
// 		redir->out_files = NULL;
// 	}
// 	free(redir);
// }

// void	free_job(t_job *job)
// {
// 	if (job->args)
// 	{
// 		free_str_arr(job->args);
// 		job->args = NULL;
// 	}
// 	if (job->redir)
// 	{
// 		free_redir(job->redir);
// 		job->redir = NULL;
// 	}
// 	free(job);
// }



void free_redir(t_redir *redir)
{
    if (!redir)
        return;

    if (redir->in_files)
        free_str_arr(redir->in_files);
    if (redir->out_files)
        free_str_arr(redir->out_files);
    if (redir->appends)
        free_str_arr(redir->appends);
    if (redir->eof)
        free_str_arr(redir->eof);
    if (redir->files_order)
        free_str_arr(redir->files_order);

    free(redir); // `t_redir` yapısını serbest bırak
}


void free_job(t_job *job)
{
    if (!job)
        return;

    if (job->args)
        free_str_arr(job->args); // `args` dinamik belleği serbest bırak
    if (job->redir)
        free_redir(job->redir);  // `t_redir` yapısını serbest bırak

    free(job); // `t_job` yapısını serbest bırak
}



void free_jobs_list(t_job *job_list)
{
    t_job *temp;

    while (job_list)
    {
        temp = job_list->next_job; // Sonraki düğümü sakla
        free_job(job_list);        // Mevcut düğümü serbest bırak
        job_list = temp;           // Bir sonraki düğüme geç
    }
}



void free_mshell(t_mshell *mshell)
{
    if (!mshell || !mshell->jobs)
        return;

    free_jobs_list(mshell->jobs->job_list); // Tüm job'ları serbest bırak
    free(mshell->jobs);                    // `t_jobs` yapısını serbest bırak
}
