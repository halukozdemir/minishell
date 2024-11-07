#include "../minishell.h"

void	check_quotes(char c, bool *sq, bool *dq)
{
	if (c == '\'' && !*dq)
		*sq = !*sq;
	if (c == '\"' && !*sq)
		*dq = !*dq;
}

void	replace_dollar_with_value_or_remove(char **input, char *key,
			char *value, int start, int end, bool needs_quotes)
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

void	process_key(char **input_ptr, t_env *env, int *i, bool in_single_quotes)
{
	int		j;
	char	*key;
	char	*value;

	if (in_single_quotes)
	{
		(*i)++;
		return;
	}

	j = *i;
	while (ft_isalnum((*input_ptr)[j]) || (*input_ptr)[j] == '_')
		j++;
	key = ft_substr(*input_ptr, *i, j - *i);
	value = get_env_value(env, key);

	if (!value && ft_strlen(key) > 0)
	{
		replace_dollar_with_value_or_remove(input_ptr, "", NULL, *i - 1, j, false);
		*i = *i - 1;
	}
	else if (value)
	{
		replace_dollar_with_value_or_remove(input_ptr, key, value, *i - 1, j, false);
		*i = *i + ft_strlen(value) - 1;
	}

	free(key);
}

void	get_dollar(char **input_ptr, t_jobs *jobs)
{
	char	*input;
	char	*new_input;
	int		i = 0, j;
	int		len = 0;
	int		total_len = 0;
	bool	in_single_quotes = false;
	bool	in_double_quotes = false;
	char	*key;
	char	*value;

	input = *input_ptr;

	while (input[i])
	{
		check_quotes(input[i], &in_single_quotes, &in_double_quotes);
		if (input[i] == '$' && !in_single_quotes)
		{
			if (!ft_strncmp(input + i, "$?", 2))
			{
				value = ft_itoa(jobs->mshell->status);
				if (value)
					total_len += ft_strlen(value);
				i += 2;
				continue;
			}
			i++;
			j = i;
			while (ft_isalnum(input[j]) || input[j] == '_')
				j++;
			key = ft_substr(input, i, j - i);
			value = get_env_value(jobs->env, key);
			if (value)
				total_len += ft_strlen(value);
			free(key);
			i = j;
		}
		else
		{
			total_len++;
			i++;
		}
	}

	new_input = (char *)malloc(total_len + 1);
	if (!new_input)
		return;

	i = 0;
	len = 0;

	while (input[i])
	{
		check_quotes(input[i], &in_single_quotes, &in_double_quotes);
		if (input[i] == '$' && !in_single_quotes)
		{
			if (!ft_strncmp(input + i, "$?", 2))
			{
				value = ft_itoa(jobs->mshell->status);
				if (value)
				{
					ft_strlcpy(new_input + len, value, ft_strlen(value) + 1);
					len += ft_strlen(value);
					i += 2;
					free(value);
					continue;
				}
			}
			else if (!ft_isalnum(input[i + 1]) && input[i + 1] != '_')
				new_input[len++] = input[i++];
			else
			{
				j = i + 1;
				while (ft_isalnum(input[j]) || input[j] == '_')
					j++;
				key = ft_substr(input, i + 1, j - i - 1);
				value = get_env_value(jobs->env, key);
				free(key);
				if (value)
				{
					ft_strlcpy(new_input + len, value, ft_strlen(value) + 1);
					len += ft_strlen(value);
				}
				i = j;
			}
		}
		else
			new_input[len++] = input[i++];
	}
	new_input[len] = '\0';
	free(*input_ptr);
	*input_ptr = new_input;
}
