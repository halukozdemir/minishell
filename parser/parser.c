#include "../minishell.h"

static char	**free_array(char **ptr, int i)
{
	while (i > 0)
	{
		i--;
		free(ptr[i]);
	}
	free(ptr);
	return (0);
}

int word_counter(char *input)
{
    int i;
    int count;
    bool in_quote;
    bool in_word;
    char *special_chars;
	char const	*quotes = "'\"";
	char	quote_type;


    special_chars = "<>| \0";
    i = 0;
    count = 0;
    in_quote = false;
    in_word = false;

    while (input[i] == ' ')
        i++;
    while (input[i])
    {
        if (ft_strchr(quotes, input[i]))
		{
			if (in_quote == false)
			{
				quote_type = input[i];
				in_quote = !in_quote;
			}
			else
				if (input[i] == quote_type)
            		in_quote = !in_quote;
		}
        if (!in_quote)
        {
            if (ft_strchr(special_chars, input[i]))
            {
                if (input[i] != ' ')
                    count++;
                if ((input[i] == '<' || input[i] == '>') && input[i] == input[i + 1])
                    i++;
                in_word = false;
            }
            else if (!in_word)
            {
                in_word = true;
                count++;
            }
        }
        i++;
    }
    printf("%d-*-*-*-\n", count);
    if (in_quote)
        syntax_error();
    return count;
}

char	*put_word(char *word, char *input, int start, int word_len)
{
	int j = 0;
	int k = start;

	while (word_len > 0)
	{
		word[j] = input[k];
		j++;
		k++;
		word_len--;
	}
	word[j] = '\0';
	return (word);
}

char **split_words(char *input, char **str, unsigned int word_count)
{
    int i = 0;
    int word = 0;
    int word_len = 0;
    bool in_quote = false;
    char *special_chars = "<>| \0";
    char const	*quotes = "'\"";
    char    quote_type;


    while (word < word_count)
    {
        while (input[i] && input[i] == ' ' && !in_quote)
            i++;

        int start = i;
        while (input[i] && (!ft_strchr(special_chars, input[i]) || in_quote))
        {
            if (ft_strchr(quotes, input[i]))
            {
                if (in_quote == false)
                {
                    quote_type = input[i];
                    in_quote = !in_quote;
                }
                else
                    if (input[i] == quote_type)
                        in_quote = !in_quote;
            }
            word_len++;
            i++;
        }

        if (ft_strchr(special_chars, input[i]) && !in_quote && word_len == 0)
        {
            word_len = 1;
            if(input[i] == input[i + 1])
            {
                i++;
                word_len = 2;
            }
            i++;
        }

        str[word] = (char *)malloc((sizeof(char) * (word_len + 1)));
        if (!str[word])
            return free_array(str, word);
        put_word(str[word], input, start, word_len);
        word_len = 0;
        word++;
    }
    str[word] = 0;
    return str;
}

char	**split_by_real_spaces(char *input)
{
	unsigned int	word_count;
	char			**str;
	int 			i;

	i = 0;
	word_count = word_counter(input);
    if (word_count == -1)
        return (NULL);
	str = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!str)
		return (0);
	str = split_words(input, str, word_count);
	while (str[i])
	{
		printf("%s\n", str[i]);
		i++;
	}
	return (str);
}
