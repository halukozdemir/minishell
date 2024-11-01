#include "../minishell.h"

void	skip_space_rev(char *input, int *i)
{
	*i = *i - 1;
	while (input[*i] == ' ' || input[*i] == '\t' || input[*i] == '\v'
		|| input[*i] == '\f' || input[*i] == '\r')
		(*i)--;
}

int	check_doublemeta(char *input, int *i)
{
		printf("gireeeer\n");
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
	printf("giriyooooooooooo\n");
		*i++;
		return 1;

	}
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		*i++;
		return 1;
	}
	//printf("input[i] = %c\n", input[i]);
	return 2;
}

void	skip_space(char *input, int *i, int *is_space)
{
	while (input[*i] == ' ' || input[*i] == '\t' || input[*i] == '\v'
		|| input[*i] == '\f' || input[*i] == '\r')
	{
		*is_space = 1;
		(*i)++;
	}
}

int	check_input(char *input)
{
	int i;
	int is_space;

	is_space = 0;
	i = 0;
	skip_space(input, &i, &is_space);
	if (is_meta(input, i) == 1 || input[i] == '\0')
	{
		return 1;

	}
	while(input[i])
	{
		//printf("input[i] = %c\n", input[i]);
		printf("input[i] = %c\n", input[i]);
		if ((is_meta(input, i) == 1 && is_meta(input, i + 1)) == 1 && check_doublemeta(input, &i) != 1)
		{
			printf("\n-----------DEBUG----------\n");
			return 1;
		} 
		skip_space(input, &i, &is_space);
		if (input[i] == '\0')
		{
			skip_space_rev(input, &i);
			if (is_meta(input, i))
			{
				return 1;

			}
		}
		if (is_space != 1)
			i++;
	}
	return -1;
}

int	syntax_cont(char *input)
{
	if(!input)
		return (0);
	if (check_input(input) == 1)
	{
		printf("syntax error\n");
		return 1;
	}
	return 0;
}
