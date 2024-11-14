#include "../minishell.h"

void	env(t_env *env)
{
	t_env *env_list;

	env_list = env;
    while (env_list)
    {
        write(1, env_list->key, ft_strlen(env_list->key));
        write(1, "=", 1);
        write(1, env_list->value, ft_strlen(env_list->value));
        write(1, "\n", 1);
        env_list = env_list->next;
    }
}
