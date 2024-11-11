/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:33:19 by halozdem          #+#    #+#             */
/*   Updated: 2024/10/30 17:37:48 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**free_array(char **ptr, int i)
{
	while (i > 0)
	{
		i--;
		free(ptr[i]);
	}
	free(ptr);
	return (NULL);
}

void	init_split(t_split *s)
{
	s->i = 0;
	s->count = 0;
	s->in_quote = false;
	s->in_word = false;
	s->word_len = 0;
	s->start = 0;
}

int	skip_whitespace(char *input, int i, t_split *s)
{
	while (input[i] && input[i] == ' ' && !s->in_quote)
		i++;
	return (i);
}

char	*put_word(char *word, char *input, int start, int word_len)
{
	int	j;
	int	k;

	j = 0;
	k = start;
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

char	**split_words(char *input, char **str, unsigned int word_count)
{
	t_split		s;
	int	word;

	init_split(&s);
	word = 0;
	while (word < (int) word_count)
	{
		s.i = skip_whitespace(input, s.i, &s);
		s.start = s.i;
		while (input[s.i] && (!ft_strchr(SPECIAL_CHARS, input[s.i])
				|| s.in_quote))
			process_quotes_and_length(&s, input);
		handle_word_allocation(input, str, &s, word);
		s.word_len = 0;
		word++;
	}
	str[word] = 0;
	return (str);
}

