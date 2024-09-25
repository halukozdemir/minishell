#include "../minishell.h"

int	is_meta(int c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

char	**put_string(char **new_str, char **str)
{
	int i;
	int j;
	int k;
	int start;

	i = 0;
	j = 0;
	k = 0;
	start = 0;
	while (str[i])
	{
		while (str[i][j])
		{
			while (str[i][j] && !is_meta(str[i][j]))
			{
				if (is_meta(str[i][j+1]))
				{
					new_str[k] = ft_substr(str[i], start, j - start + 1);
					k++;
					j++;
					start = j;
					break ;
					printf("%s", new_str[k]);
				}
				else if (str[i][j+1] == '\0')
				{
					new_str[k] = ft_substr(str[i], start, j - start + 1);
					k++;
					j++;
					break ;
				}
				j++;
			}
			if (is_meta(str[i][j]))
			{
				if ((str[i][j] == '<' && str[i][j+1] == '<') || (str[i][j] == '>' && str[i][j+1] == '>'))
				{
					new_str[k] = ft_substr(str[i], j, 2);
					k++;
					j += 2;
					start = j;
				}
				else
				{
					new_str[k] = ft_substr(str[i], j, 1);
					k++;
					j++;
					start = j;
				}
			}
			// j++;
		}
		i++;
		start = 0;
		j = 0;
	}
	k = 0;
	return (new_str);
}

int 	while_loop(char **str, int i, int j, int word_count)
{
	int n = 0;

	while (str[i][j])
	{
		while (ft_isprint(str[i][j]) && !is_meta(str[i][j]))
		{
			if (is_meta(str[i][j+1]) || str[i][j+1] == '\0')
			{
				word_count++;
				break ;
			}
			n = 1;
			j++;
		}
		if (n == 0)
			j++;
		if (is_meta(str[i][j]))
		{
			word_count++;
			if ((str[i][j] == '<' && str[i][j+1] == '<')
			|| (str[i][j] == '>' && str[i][j+1] == '>'))
				j += 2 ;
			j++;
		}
		j++;
	}
	return (word_count);
}

char	**split_by_meta(char **str)
{
	int	i;
	int	j;
	int k = 0;
	int	word_count;
	char	**new_str;

	i = 0;
	j = 0;
	word_count = 0;
	while (str[i])
	{
		word_count = while_loop(str, i, j, word_count);
		i++;
		j = 0;
	}
	new_str = (char **)malloc(sizeof(char *) * (word_count + 1));
	new_str[word_count + 1] = NULL;
	new_str = put_string(new_str, str);
	while (new_str[k])
	{
		printf("%s\n", new_str[k]);
		k++;
	}
	printf("meta karakterlere göre ayrılmışken = %d\n_______________________________________\n", word_count);
	return (new_str);
}
