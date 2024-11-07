#include "../minishell.h"

void	env(t_env *env)
{
	t_env *env_list;

	env_list = env;
    while (env_list)
    {
        printf("%s=%s\n", env_list->key, env_list->value);
        env_list = env_list->next;
    }
}
