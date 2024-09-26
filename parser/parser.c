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
    int i = 0;
    int count = 0;
    bool in_quote = false;
    bool in_word = false;
    char *special_chars = "<>| \0";  // Özel karakterler

    while (input[i] == ' ') // Baştaki boşlukları atla
        i++;

    while (input[i])
    {
        if (input[i] == 34)  // Çift tırnak karakteri
            in_quote = !in_quote;

        if (!in_quote)
        {
            if (ft_strchr(special_chars, input[i]))  // Eğer özel bir karaktere denk gelirsen
            {
                if (input[i] != ' ')  // Boşluk değilse, bu bir kelimedir
                    count++;
                in_word = false;
            }
            else if (!in_word)  // Yeni bir kelime başlıyorsa
            {
                in_word = true;
                count++;
            }
        }
        i++;
    }

    printf("%d-*-*-*-\n", count);
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
    char *special_chars = "<>| \0";  // Özel karakterler

    while (word < word_count)
    {
        while (input[i] && input[i] == ' ' && !in_quote)  // Boşlukları atla
            i++;

        int start = i;
        while (input[i] && (!ft_strchr(special_chars, input[i]) || in_quote))  // Özel karaktere denk gelene kadar kelimeyi oku
        {
            if (input[i] == 34)  // Çift tırnak
                in_quote = !in_quote;
            word_len++;
            i++;
        }

        if (ft_strchr(special_chars, input[i]) && !in_quote && word_len == 0)  // Özel bir karakter bulduysan
        {
            word_len = 1;  // Özel karakter tek başına bir kelime olmalı
            i++;
        }

        str[word] = (char *)malloc((sizeof(char) * (word_len + 1)));
        if (!str[word])
            return free_array(str, word);  // Eğer malloc başarısız olursa, belleği temizle
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
