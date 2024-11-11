#include "../minishell.h"

void	echo(char **args)
{
	int	i;
	int	j;
	int	newline;
	char	quote_type;
	bool	in_quote;

	newline = 1;
	in_quote = false;
	i = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0 && ft_strlen(args[1]) == 2)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (ft_strchr(QUOTES, args[i][j]))
			{
				if (!in_quote)
				{
					quote_type = args[i][j];
					in_quote = !in_quote;
				}
				else
				{
					if (args[i][j] == quote_type)
						in_quote = !in_quote;
				}
			}
			if (args[i][j] != quote_type)
				printf("%c", args[i][j]);
			j++;
		}
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
