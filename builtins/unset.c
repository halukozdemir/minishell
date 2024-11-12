#include "../minishell.h"

void	unset(t_env **env, char **args)
{
	t_env	*temp;
	int		len_key;
	int		len;
	int		i;

	i = 1;
	while (args[i])
	{
		temp = *env;
		len_key = ft_strlen(args[i]);
		while (temp)
		{
			len = ft_strlen(temp->key);
			if (!ft_strncmp(temp->key, args[i], len) && len == len_key)
			{
				if (temp->next)
					temp->next->prev = temp->prev;
				if (temp->prev)
					temp->prev->next = temp->next;
				if (temp == *env)
					*env = (*env)->next;
				free(temp->key);
				free(temp->value);
				free(temp);
				return ;
			}
			temp = temp->next;
		}
		i++;
	}
}
