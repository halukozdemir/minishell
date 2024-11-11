#include "../minishell.h"

int	is_meta(char *input, int i)
{
	if (input[i] == '<' || input[i] == '>')
	{
		while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
			|| input[i] == '\f' || input[i] == '\r')
			i++;
		return 1;
	}
	return 0;
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\v'
		|| c == '\f' || c == '\r')
		return 1;
	return 0;
}
