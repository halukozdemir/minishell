#include "../minishell.h"

int	pipe_cont(char *input)
{
	int i;
	bool	in_quote;
	char const	*quotes = "'\"";
	char    quote_type;

	in_quote = false;
	i = 0;
	while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
		|| input[i] == '\f' || input[i] == '\r')
		i++;
	if (input[i] == '|')
		return 1;
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
		if (input[i] == '|' && in_quote == false)
		{
			i++;
			while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
				|| input[i] == '\f' || input[i] == '\r')
				i++;
			if (input[i] == '|' || input[i] == '<' || input[i] == '>')
				return 1;
			if (input[i + 1] && input[i + 1] == '\0')
				return 1;
		}
		i++;
	}
	return (0);
}

int	redir_cont2(char *input, int i, char c)
{
	bool	in_quote;
	char const	*quotes = "'\"";
	char    quote_type;

	in_quote = false;
	i++;
	while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
		|| input[i] == '\f' || input[i] == '\r')
		i++;
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
	if (input[i] == c && in_quote == false)
		return 1;
	if (input[i] == '\0' && in_quote == false)//bakılacak
		return 1;
	return (0);
}

int	redir_cont(char *input)
{
	int i;
	bool	in_quote;
	char const	*quotes = "'\"";
	char    quote_type;

	in_quote = false;
	i = 0;
	while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
		|| input[i] == '\f' || input[i] == '\r')
		i++;
	if ((input[i] == '>' || input[i] == '<') && in_quote == false)
		return 1;
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
		if (input[i] == '>' && in_quote == false)
		{
			while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
				|| input[i] == '\f' || input[i] == '\r')
				i++;
			if (input[i + 1] && (input[i + 1] == '|' || input[i + 1] == '<' || input[i + 1] == '>'))
				return 1;
			if (input[i + 1] && input[i+1] == '>' && input[i+2] == '\0') //buraya bakılacak
				return 1;
			redir_cont2(input, i, '<');
		}
		else if (input[i] == '<' && in_quote == false)
		{
			while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
				|| input[i] == '\f' || input[i] == '\r')
				i++;
			if (input[i] == '|' || input[i] == '<' || input[i] == '>')
				return 1;
			if (input[i + 1] && input[i+1] == '<' && input[i+2] == '\0')
				return 1;
			redir_cont2(input, i, '>');
		}
		i++;
	}
	return (0);
}

int	quote_cont(char *input)
{
	char const	*quotes = "'\"";
	char    quote_type;
	int		i;
	bool	in_quote;

	in_quote = false;
	i = 0;
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
		i++;
	}
	if (in_quote)
		return 1;
	else
		return 0;
}

int	syntax_cont(char *input, bool *has_error)
{
	int i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
		|| input[i] == '\f' || input[i] == '\r')
		i++;
	if (input[i] == '\0')
		return (0);
	if (pipe_cont(input) || redir_cont(input) || quote_cont(input))
	{
		*has_error = true;
		return 0;
	}
	return (1);
}
