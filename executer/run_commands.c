#include "../minishell.h"

void	run_cmds(char *input, t_env *env)
{
	if (ft_strncmp(input, "exit", 4) == 0)
		exit(0);
	if (ft_strncmp(input, "env", 3) == 0)
		run_env(env);
}
