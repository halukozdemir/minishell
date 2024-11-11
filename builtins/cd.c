#include "../minishell.h"

char	cd(t_mshell *mshell, char *path)
{
	if (chdir(path) == -1)
	{
		mshell->status = 1;
		perror("cd");
	}
	return (EXIT_SUCCESS);
}
