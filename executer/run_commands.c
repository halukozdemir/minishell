#include "../minishell.h"

void	run_cmds(char *input, char **env)
{
	if (ft_strncmp(input, "exit", 4) == 0)
		exit(0);
	if (ft_strncmp(input, "env", 3) == 0)
		envfunc(env, 1);
}
