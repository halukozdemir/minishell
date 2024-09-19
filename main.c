#include "minishell.h"
#include <string.h>

int main(int argc, char **argv, char **env)
{
    t_cmd   *cmd;

    (void)argv;
    if (argc != 1)
    {
        printf("Argument error. \n");
        exit(1);
    }
	char *input;

    while (1) 
	{
        input = readline("minishell> ");
        if (!input)
            break;
		add_history(input);
		printf("%s\n", input);
		if (strncmp(input, "exit", 4) == 0)
			exit(0);
		if (strncmp(input, "env", 3) == 0)
		{
			envfunc(env);
		}
    }

    cmd = (t_cmd *)malloc(sizeof(t_cmd));
    if (!cmd)
        return (0);
    init_struct(cmd);
}
