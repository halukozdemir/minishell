#include "../minishell.h"

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
		rtrn[i] = str_arr[i];
	rtrn[i] = ft_strdup(element);
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
			return ((free_env_array(splitted_path)), temp2);//splitted pathi freeleyecek
		free(temp);
		free(temp2);
		i++;
	}
	return (NULL);
}
