#include "../minishell.h"
#include <limits.h>

// Yardımcı fonksiyon: tırnakları kaldırır
char *strip_quotes(char *str)
{
    char *new_str;
    int i = 0, j = 0;
    int len = ft_strlen(str);

    new_str = malloc(len + 1);
    if (!new_str)
        return (NULL);

    while (str[i])
    {
        if (str[i] != '"' && str[i] != '\'')
        {
            new_str[j] = str[i];
            j++;
        }
        i++;
    }
    new_str[j] = '\0';
    return (new_str);
}

char exit_error(char *args, const char *message, t_mshell *mshell)
{
    write(2, "minishell: exit: ", 17);
    write(2, args, ft_strlen(args));
    write(2, ": ", 2);
    write(2, message, ft_strlen(message));
    write(2, "\n", 1);
    mshell->status = 255;
    return (EXIT_FAILURE);
}

char is_all_digit(char *str)
{
    int i = 0;

    if (!str || !str[i])
        return (EXIT_FAILURE);
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (EXIT_FAILURE);
        i++;
    }
    return (EXIT_SUCCESS);
}

int get_exit_value(char *str)
{
    long long result;
    int i;
    int sign;

	result = 0;
	i = 0;
	sign = 1;
    while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (EXIT_FAILURE);
        if (result > (LLONG_MAX - (str[i] - '0')) / 10)
            return (EXIT_FAILURE);
        result = result * 10 + (str[i] - '0');
        i++;
    }
    result *= sign;
    result %= 256;
    if (result < 0)
        result += 256;
    return ((int)result);
}

void exit_d(t_mshell *mshell, char **args)
{
    int exit_status;
    char *stripped_arg;

    printf("exit\n");

    if (args[1] != NULL)
    {
        stripped_arg = strip_quotes(args[1]);
        if (!stripped_arg)
        {
            exit_error(args[1], "memory allocation error", mshell);
            exit(mshell->status);
        }
        if (is_all_digit(stripped_arg) == EXIT_FAILURE)
        {
            exit_error(args[1], "numeric argument required", mshell);
            free(stripped_arg);
            exit(mshell->status);
        }
        else if (args[2] != NULL)
        {
            write(2, "minishell: exit: too many arguments\n", 36);
            mshell->status = 1;
            free(stripped_arg);
            return;
        }
        else
        {
            exit_status = get_exit_value(stripped_arg);
            if (exit_status == EXIT_FAILURE)
            {
                exit_error(args[1], "numeric argument required", mshell);
                free(stripped_arg);
                exit(mshell->status);
            }
            mshell->status = exit_status;
        }
        free(stripped_arg);
    }
    else
    {
        mshell->status = 0;
    }
    exit(mshell->status);
}
