#include "../minishell.h"

void	run_env(t_env *env)
{
	while(env)
	{
		printf("%s = %s\n", env->key, env->value);
		env = env->next;
	}
}
