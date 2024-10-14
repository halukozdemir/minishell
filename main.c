#include "minishell.h"
#include <string.h>

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
			str = get_token(input);
			run_cmds(input, env);
		}
    }
	return (0);
}
