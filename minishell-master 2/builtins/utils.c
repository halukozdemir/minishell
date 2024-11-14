#include "../minishell.h"

// Yardımcı fonksiyon: tırnakları kaldırır
char	*strip_quotes(char *str)
{
	char	*new_str;
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	new_str = malloc(len + 1);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

char	is_all_digit(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (EXIT_FAILURE);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
