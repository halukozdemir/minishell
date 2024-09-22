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

char	*put_word(char *word, char *input, int i, int word_len)
{
	int	j;

	j = 0;
	while (word_len > 0)
	{
		word[j] = input[i - word_len];
		j++;
		word_len--;
	}
	word[j] = 0;
	return (word);
}

char	**split_words(char *input, char **str, unsigned int word_count)
{
	int		i;
	int		word;
	int		word_len;
	bool	in_quote;

	i = 0;
	word = 0;
	word_len = 0;
	in_quote = false;
	while (word < word_count)
	{
		while (input[i] && input[i] == ' ' && !in_quote)
			i++;
		while (input[i] && (input[i] != ' ' || in_quote))
		{
			if (input[i] == 34)
				in_quote = !in_quote;
			else if (!in_quote)
				word_len++;
			i++;
		}
		str[word] = (char *)malloc((sizeof(char) * word_len + 1));
		// if (!str)
			// return (free_array(str, word));
		put_word(str[word], input, i, word_len);
		word_len = 0;
		word++;
	}
	str[word] = 0;
	return (str);
}

char	**parser(char *input)
{
	unsigned int	word_count;
	char			**str;

	word_count = word_counter(input);
	str = 	(char **)malloc(sizeof(char *) * (word_count + 1));
	if (!str)
		return(0);
	str = split_words(input, str, word_count);
	printf("%d \n", word_count);
	return (str);
}
