/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halozdem <halozdem@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:33:19 by halozdem          #+#    #+#             */
/*   Updated: 2024/10/30 17:27:40 by halozdem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_quotes(t_split *s, char *input)
{
	if (!s->in_quote)
	{
		s->quote_type = input[s->i];
		s->in_quote = true;
	}
	else if (input[s->i] == s->quote_type)
		s->in_quote = false;
}

void	handle_special_chars(t_split *s, char *input)
{
	if (input[s->i] != ' ')
		s->count++;
	if ((input[s->i] == '<' || input[s->i] == '>') && input[s->i] == input[s->i
			+ 1])
		s->i++;
	s->in_word = false;
}

void	process_quotes_and_length(t_split *s, char *input)
{
	if (ft_strchr(QUOTES, input[s->i]))
		handle_quotes(s, input);
	s->word_len++;
	s->i++;
}

void	handle_word_allocation(char *input, char **str, t_split *s, int word)
{
	if (ft_strchr(SPECIAL_CHARS, input[s->i]) && !s->in_quote
		&& s->word_len == 0)
	{
		s->word_len = 1;
		if (input[s->i] == input[s->i + 1])
		{
			s->i++;
			s->word_len = 2;
		}
		s->i++;
	}
	str[word] = (char *)malloc(sizeof(char) * (s->word_len + 1));
	if (!str[word])
		free_array(str, word);
	put_word(str[word], input, s->start, s->word_len);
}
