#include "../minishell.h"

int	is_meta(char *input, int i)
{
	if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		return 1;
	return 0;
}

