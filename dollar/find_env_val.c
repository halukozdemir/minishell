#include "../minishell.h"

char	*find_env_val(char *key, char **env)
{
	int		i;
	int		j;
	int		len;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		len = j;
		if (ft_strncmp(key, env[i], len) == 0)
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}
