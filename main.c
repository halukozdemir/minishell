#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **env)
{
    char    *input;
    char    **tokens;
    char    **new_env;
    t_mshell shell;

    (void)argv;
    if (argc != 1)
        return 1;
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            break;
        }
        shell.env = envfunc2(env);
        add_history(input);
        get_dollar(&input, shell.env);
        new_env = env_to_array(shell.env);
        tokens = get_token(input);

        fill_jobs_from_tokens(&shell, tokens);
        // Execute built-in commands if applicable
        if (!execute_builtin(tokens, &new_env))
        {
            execute_jobs(shell.jobs);
        }

        free(input);
        int i = 0;
        while (tokens[i])
        {
            free(tokens[i]);
            i++;
        }
        free(tokens);

        free_jobs(shell.jobs);
    }

    return 0;
}
