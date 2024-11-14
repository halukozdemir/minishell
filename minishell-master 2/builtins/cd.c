#include "../minishell.h"

char	cd(char *path)
{
	if (chdir(path) == -1)
	{
		g_exit_status = 1;
		perror("cd");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
