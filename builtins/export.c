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

char	export(t_env *env, t_mshell *mshell, char *args)
{
	char	*key;
	char	*value;
	t_env	*new;
	t_env	*tmp;
	int		i;

	if (args)
	{
		i = 0;
		while (args[i] && args[i] != '=')
			i++;
		if (args[i] != '=')
		{
			if (is_alnum_or_underscore(args, 1))
				return (export_error(args, "not a valid identifier", mshell));
		}
		key = ft_substr(args, 0, i);
		if (is_alnum_or_underscore(key, 0))
			export_error(args, "not an identifier", mshell);
		if (!args[i + 1])
			export_error(args, "not a valid identifier", mshell);
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
				printf("declare -x %s=\"\"\n", tmp->key);
			tmp = tmp->next;
		}
	}
	return (EXIT_SUCCESS);
}
