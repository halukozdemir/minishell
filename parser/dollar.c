#include "../minishell.h"

void	check_quotes(char c, bool *sq, bool *dq)
{
	if (c == '\'' && !*dq)
		*sq = !*sq;
	if (c == '\"' && !*sq)
		*dq = !*dq;
}
void	replace_dollar_with_value_or_remove(char **input, char *key,
			char *value, int start, int end)
{
	char	*new_input;
	int		new_len;

	if (value)
		new_len = ft_strlen(*input) + ft_strlen(value) - (end - start);
	else
		new_len = ft_strlen(*input) - (end - start);
	new_input = (char *)malloc(new_len + 1);
	if (!new_input)
		return ;
	ft_strlcpy(new_input, *input, start + 1);
	if (value)
		ft_strlcat(new_input, value, new_len + 1);
	ft_strlcat(new_input, *input + end, new_len + 1);
	free(*input);
	*input = new_input;
}

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0
			&& ft_strlen(env->key) == ft_strlen(key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	process_key(char **input_ptr, t_env *env, int *i)
{
	int		j;
	char	*key;
	char	*value;

	j = *i;
	while (ft_isalnum((*input_ptr)[j]) || (*input_ptr)[j] == '_')
		j++;
	key = ft_substr(*input_ptr, *i, j - *i);
	value = get_env_value(env, key);
	replace_dollar_with_value_or_remove(input_ptr, key, value, *i - 1, j);
	free(key);
	*i = *i - 1;
}

void	get_dollar(char **input_ptr, t_env *env)
{
	char	*input;
	bool	sq;
	bool	dq;
	int		i;

	sq = false;
	dq = false;
	input = *input_ptr;
	i = 0;
	while (input[i])
	{
		check_quotes(input[i], &sq, &dq);
		if (input[i] == '$' && !sq)
		{
			i++;
			process_key(input_ptr, env, &i);
			input = *input_ptr;
		}
		i++;
	}
}
