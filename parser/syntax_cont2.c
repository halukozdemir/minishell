#include "../minishell.h"

int	is_meta(char *input, int i)
{
	if (input[i] == '<' || input[i] == '>')
	{
	printf("GİRİYOOOOOOO\n");
		while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
			|| input[i] == '\f' || input[i] == '\r')
			i++;
		return 1;
	}
	return 0;
}
