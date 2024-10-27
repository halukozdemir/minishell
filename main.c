#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Function to check and execute built-in commands
int execute_builtin(char **args, char ***new_env)
{
    if (ft_strncmp(args[0], "echo", 4) == 0)
    {
        ft_echo(args);
        return 1;
    }
    else if (ft_strncmp(args[0], "cd", 2) == 0)
    {
        ft_cd(args, *new_env);
        return 1;
    }
    else if (ft_strncmp(args[0], "pwd", 3) == 0)
    {
        ft_pwd();
        return 1;
    }
    else if (ft_strncmp(args[0], "export", 6) == 0)
    {
        ft_export(args, new_env);
        return 1;
    }
    else if (ft_strncmp(args[0], "unset", 5) == 0)
    {
        ft_unset(args, new_env);
        return 1;
    }
    else if (ft_strncmp(args[0], "env", 3) == 0)
    {
        ft_env(*new_env);
        return 1;
    }
    else if (ft_strncmp(args[0], "exit", 4) == 0)
    {
        ft_exit();
        return 1;
    }
    return 0;
}

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
