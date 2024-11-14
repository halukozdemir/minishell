/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:33:19 by halozdem          #+#    #+#             */
/*   Updated: 2024/10/30 17:26:59 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_initial_spaces(t_split *s, char *input)
{
	while (input[s->i] == ' ' || input[s->i] == '\t' || input[s->i] == '\v'
		|| input[s->i] == '\f' || input[s->i] == '\r')
		s->i++;
}

int	word_counter(char *input)
{
	t_split	s;

	s.i = 0;
	s.count = 0;
	s.in_quote = false;
	s.in_word = false;
	handle_initial_spaces(&s, input);
	while (input[s.i])
	{
		if (ft_strchr(QUOTES, input[s.i]))
			handle_quotes(&s, input);
		if (!s.in_quote)
		{
			if (ft_strchr(SPECIAL_CHARS, input[s.i]))
				handle_special_chars(&s, input);
			else if (!s.in_word)
			{
				s.in_word = true;
				s.count++;
			}
		}
		s.i++;
	}
	return (s.count);
}

char	**get_token(char *input)
{
	unsigned int	word_count;
	char			**str;

	word_count = word_counter(input);
	if ((int) word_count == -1)
		return (NULL);
	str = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!str)
		return (NULL);
	str = split_words(input, str, word_count);
	return (str);
}

