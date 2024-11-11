#include "../minishell.h"

char	exit_error(char *args, const char *message, t_mshell *mshell)
{
	write(2, "minishell: exit: `", 19);
	write(2, args, ft_strlen(args));
	write(2, "': ", 3);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	mshell->status = 255;
	return (EXIT_FAILURE);
}

char	is_all_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			i++;
		else
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	exit_d(t_mshell *mshell, char **args)
{
	int	exit_status;

	exit_status = 0;
	if (args[1] && is_all_digit(args[1]))
	{
		printf("exit\n");
		exit_error(args[1], "numeric argument required", mshell);
		exit(mshell->status);
		return ;
	}
	if (args[1])
	{
		if (args[2])
		{
			printf("exit: too many arguments\n");
			return ;
		}
		exit_status = ft_atoi(args[1]);
		mshell->status = exit_status;
		printf("exit\n");
		exit(mshell->status);
	}
	else
	{
		exit_status = 0;
		printf("exit\n");
		exit(mshell->status);
	}
}
