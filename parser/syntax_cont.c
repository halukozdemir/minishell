#include "../minishell.h"

void	syntax_error(void)
{
	printf("syntax error!!!\n");
}

void	pipe_cont(input)
{
	int i;

	i = 0;
//BURADA KALMIŞTIK BOSLUKLARI GECİP PİPE İLE Mİ BAŞLIYOR DİE BAKCAK


	if (input[0])

	while (input[i])
	{
		if (input)
	}
}

int	syntax_cont(char *input)
{
	int i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
		|| input[i] == '\f' || input[i] == '\r')
		i++;
	if (input[i] == '\0')
		return (0);
	pipe_cont(input);
	return (0);
}
