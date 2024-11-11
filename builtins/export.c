#include "../minishell.h"

static char	is_alnum_or_underscore(char *str, char state)
{
	char	contain_alpha;
	int		i;

	contain_alpha = 0;
	i = 0;
	while (str[i])
	{
		if (state)
		{
			if (str[i] == '=')
				i++;
		}
		if (ft_isalpha(str[i]))
			contain_alpha = 1;
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (EXIT_FAILURE);
		i++;
	}
	if (!contain_alpha)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	export_error(char *args, const char *message, t_mshell *mshell)
{
	write(2, "minishell: export: `", 21);
	write(2, args, ft_strlen(args));
	write(2, "': ", 3);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	mshell->status = 1;
	return (EXIT_FAILURE);
}

char	export(t_env	*env, t_mshell *mshell, char **args)
{
	char	*key;
	char	*value;
	t_env	*new;
	t_env	*tmp;
	int		i;
	int		j;

	i = 1;
	if (args[i])
	{
		while (args[i])
		{
			j = 0;
			while (args[i][j] && args[i][j] != '=')
				j++;
			key = ft_substr(args[i], 0, j);
			if (is_alnum_or_underscore(key, 0))
				return (export_error(args[i], "not a valid identifier", mshell));
			value = ft_substr(args[i], j + 1, ft_strlen(args[i]) - j);
			new = (t_env *)malloc(sizeof(t_env));
			new->key = ft_strdup(key);
			free(key);
			if (value)
				new->value = ft_strdup(value);
			else
				new->value = ft_strdup("");
			free(value);
			lstadd_back2(&env, new);
			i++;
		}
	}
	else
	{
		tmp = env;
		while (tmp)
		{
			if (tmp->value[0])
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			else
				printf("declare -x %s=\"\"\n", tmp->key);
			tmp = tmp->next;
		}
	}
	return (EXIT_SUCCESS);
}
