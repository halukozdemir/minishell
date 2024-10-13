#include "minishell.h"
#include <string.h>

int main(int argc, char **argv, char **env)
{
	char 	*input;
	char	**str;

	(void)argv;
	if (argc != 1)
	{
		printf("Argument error. \n");
		exit(1);
	} 
    while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break;
		add_history(input);
		syntax_cont(input);
		str = split_by_real_spaces(input);
		run_cmds(input, env);
    }
	return (0);
}
