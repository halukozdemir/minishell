#include "minishell.h"

int		word_counter(char *input)
{
	int	i;
	int	count;
	bool	in_quote;
	bool	in_word;

	i = 0;
	count = 0;
	in_quote = false;
	in_word = false;
	while(input[i] == ' ')
		i++;
	while(input[i])
	{
		if (input[i] == 34)
			in_quote = !in_quote;
		if (!in_quote)
		{
			if (input[i] == ' ')
				in_word = false;
			else if (in_word == false)
			{
				in_word = 1;
				count++;
			}
		}
	i++;
	}
	return (count);
}

void	parser(char *input)
{
	unsigned int	word_count;

	word_count = word_counter(input);
	printf("%d \n", word_count);
}
