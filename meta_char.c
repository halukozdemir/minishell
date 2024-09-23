#include "minishell.h"

int	is_meta(int c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	else
		return (0);
}

char	**split_by_meta(char **str)
{
	int	i;
	int	j;
	int	word_count;

	i = 0;
	j = 0;
	word_count = 0;

	while(str[i])
	{
		while (str[i][j])
		{
			while (ft_isprint(str[i][j]) && !is_meta(str[i][j]))
			{
				j++;
				if (is_meta(str[i][j+1]) || str[i][j+1] == '\0')
				{
					word_count++;
					break;
				}
			}
			if (is_meta(str[i][j]))
			{
				word_count++;
				if ((str[i][j] == '<' && str[i][j+1] == '<') || (str[i][j] == '>' && str[i][j+1] == '>'))
					j+=2;
				j++;
			}
			/*while (ft_isprint(str[i][j]) && !is_meta(str[i][j]))
			{
				j++;
				if (is_meta(str[i][j+1]))
				{
					word_count++;
					break;
				}
			}*/
			j++;
		}
		i++;
		j = 0;
	}
	printf("%d\n", word_count);
	return (str);
}

