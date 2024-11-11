#include "../minishell.h"

static void	del_node(t_env *node)
{
	// printf("prev: %s\n next: %s\n current: %s\n", node->prev->key, node->next->key, node->key);
	if (node->next)
		node->next->prev = node->prev;
	if (node->prev)
		node->prev->next = node->next;
	// printf("next: %s\n prev: %s\n current: %s\n", node->prev->next->key, node->next->prev->key, node->key);
	free(node->key);
	free(node->value);
	free(node);
}

static void	del_key(t_env *env, char *key)
{
	t_env	*temp;
	int		len_key;
	int		len;

	temp = env;
	len_key = ft_strlen(key);
	while (temp)
	{
		len = ft_strlen(temp->key);
		if (!ft_strncmp(temp->key, key, len) && len == len_key)
		{
			del_node(temp);
			return ;
		}
		temp = temp->next;
	}
}

void	unset(t_env *env, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		del_key(env, args[i]);
		i++;
	}
	// printf("wqdsad\n");

}
