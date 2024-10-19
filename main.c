#include "minishell.h"
#include <string.h>

void	get_dollar(char *input)
{
	bool 	dq;
	bool 	sq;
	bool	has_dollar;
	int		i;
	int		j;
	char	*key;

	dq = false;
	sq = false;
	has_dollar = false;
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '\"' && sq == false)
		{
			dq = !dq;
			i++;
		}
		else if (input[i] == '\'' && dq == false)
		{
			sq = !sq;
			i++;
		}
		while (input[i] && input[i] != '$')
			i++;
		if (input[i] == '$')
			has_dollar = true;
		while (input[i] && dq && has_dollar == true)
		{
		printf("sadsddsadsadsad3291213213\n");
			key[j] = input[i];
			i++;
			j++;
		}
		printf("key: %s\n", key);

	}

}

int main(int argc, char **argv, char **env)
{
	char 	*input;
	char	**str;
	bool	has_error;

	(void)argv;
	if (argc != 1)
	{
		printf("Argument error. \n");
		exit(1);
	}
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
			get_dollar(input);
			str = get_token(input);
			run_cmds(input, env);
		}
    }
	return (0);
}
