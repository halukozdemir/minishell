#include "../minishell.h"

char	export(t_env *env, char *args)
{
	int	i;
	char *key;
	char *value;
	t_env *new;
	t_env *tmp;
	if (args)
	{
		i = 0;
		while (args[i] != '=')
			i++;
		key = ft_substr(args, 0, i);
		value = ft_substr(args, i + 1, ft_strlen(args) - i);
		new = (t_env *)malloc(sizeof(t_env));
		new->key = ft_strdup(key);
		new->value = ft_strdup(value);
		lstadd_back2(&env, new);
	}
	else
	{
		tmp = env;
		while (tmp)
		{
			if (tmp->value[0])
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			else
				printf("declare -x %s\n", tmp->key);
			tmp = tmp->next;
		}
	}
	return (EXIT_SUCCESS);
}
