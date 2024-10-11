#include "minishell.h"
#include <string.h>

int		builtinfunc(char *input, char **env)
{
	t_env **lst;
	t_env *temp;

	if (strncmp(input, "exit", 4) == 0)
		exit(0);
	if (strcmp(input, "env") == 0)
		envfunc(env, 1);
	if (strcmp(input, "pwd") == 0)
	{
		lst = (t_env **)malloc(sizeof(t_env));
		lst = envfunc(env, 0);
		temp = *lst;
		while (temp)
		{
			if (strcmp(temp->key, "PWD") == 0)
			{
				printf("%s\n", temp->value);
				break;
			}
			temp = temp->prev;
			printf("%s\n", temp->key);
		}
	}
	return (0);
}

int main(int argc, char **argv, char **env)
{
	t_command   *cmd;
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

		str = split_by_real_spaces(input);
		builtinfunc(input, env);
    }
    cmd = (t_command *)malloc(sizeof(t_command));
    if (!cmd)
        return (0);
    init_struct(cmd);
}
