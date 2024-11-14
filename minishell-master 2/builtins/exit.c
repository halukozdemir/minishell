#include "../minishell.h"
#include <limits.h>

char	exit_error(char *args, const char *message)
{
	write(2, "minishell: exit: ", 17);
	write(2, args, ft_strlen(args));
	write(2, ": ", 2);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	g_exit_status = 255;
	return (EXIT_FAILURE);
}

void	get_exit_val_loop(int c, long long result)
{
	if (c < '0' || c > '9')
		return (EXIT_FAILURE);
	if (result > (LLONG_MAX - (c - '0')) / 10)
		return (EXIT_FAILURE);
	result = result * 10 + (c - '0');
}

int	get_exit_value(char *str)
{
	long long	result;
	int			i;
	int			sign;

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
		get_exit_val_loop(str[i], result);
		i++;
	}
	result *= sign;
	result %= 256;
	if (result < 0)
		result += 256;
	return ((int)result);
}

void	exit_d_2(char *stripped_arg, char **args, int exit_status)
{
	if (is_all_digit(stripped_arg) == EXIT_FAILURE)
	{
		exit_error(args[1], "numeric argument required");
		free(stripped_arg);
		exit(g_exit_status);
	}
	else if (args[2] != NULL)
	{
		g_exit_status = 1;
		write(2, "minishell: exit: too many arguments\n", 36);
		free(stripped_arg);
		return ;
	}
	else
	{
		exit_status = get_exit_value(stripped_arg);
		if (exit_status == EXIT_FAILURE)
		{
			exit_error(args[1], "numeric argument required");
			free(stripped_arg);
			exit(g_exit_status);
		}
		g_exit_status = exit_status;
	}
}

void	exit_d(char **args)
{
	int		exit_status;
	char	*stripped_arg;

	printf("exit\n");
	if (args[1] != NULL)
	{
		stripped_arg = strip_quotes(args[1]);
		if (!stripped_arg)
		{
			exit_error(args[1], "memory allocation error");
			exit(g_exit_status);
		}
		exit_d_2(stripped_arg, args, exit_status);
		free(stripped_arg);
	}
	else
		g_exit_status = 0;
	exit(g_exit_status);
}
