#include "minishell.h"
#include <string.h>

void	get_dollar(char *input, t_env *env)
{
	bool	sq;
	bool	dq;
	int		i;
	int		j;
	char	*key;
	int		env_i;
	int		env_j;
	int		env_len;
	char	*env_val;

	i = 0;
	sq = false;
	dq = false;
	j = 0;
	while (input[i])
	{
		j = 0;
		if (input[i] == '\'' && !dq)
			sq = !sq;
		if (input[i] == '\"' && !sq)
			dq = !dq;
		if (input[i] == '$' && !sq)
		{
			i++;
			j = i;
			while ((ft_isalnum(input[j]) || input[j] == '_'))
				j++;
			key = ft_substr(input, i, j - i);
			printf("%s\n", key);
//env val bulma
		}
		i++;
	}
}


int main(int argc, char **argv, char **env)
{
	char 	*input;
	char	**str;
	bool	has_error;
	t_env	*env2;

	(void)argv;
	if (argc != 1)
	{
		printf("Argument error. \n");
		exit(1);
	}
	env2 = envfunc2(env);
	printf("-----------");
    while (1)
	{
		has_error = false;
		input = readline("minishell> ");
		if (!input)
			break;
		add_history(input);
		syntax_cont(input, &has_error);
		if (has_error == true)
			printf("Syntax error.\n");
		else
		{
			get_dollar(input, env2);
			str = get_token(input);
			run_cmds(input, env2);
		}
    }
	return (0);
}
