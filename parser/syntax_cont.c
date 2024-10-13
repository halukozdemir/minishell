#include "../minishell.h"

void	syntax_error(void)
{
	printf("syntax error!!!\n");
}

void	pipe_cont(char *input)
{
	int i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
		|| input[i] == '\f' || input[i] == '\r')
		i++;
	if (input[i] == '|')	
		syntax_error();
	while (input[i])
	{
		if (input[i] == '|')
		{
			i++;
			while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
				|| input[i] == '\f' || input[i] == '\r')
				i++;
			if (input[i] == '|')
				syntax_error();
		}
		i++;
	}
}

void	redir_cont2(char *input, int i, char c)
{
	i++;
	while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
		|| input[i] == '\f' || input[i] == '\r')
		i++;
	if (input[i] == c)
		syntax_error();
	if (input[i] == '\0')
		syntax_error();
}

void	redir_cont(char *input)
{
	int i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
		|| input[i] == '\f' || input[i] == '\r')
		i++;
	if (input[i] == '>' || input[i] == '<')
		syntax_error();
	while (input[i])
	{
		if (input[i] == '>')
		{
			if (input[i+1] == '>' && input[i+2] == '\0')
				syntax_error();
			redir_cont2(input, i, '<');
		}
		else if (input[i] == '<')
		{
			if (input[i+1] == '<' && input[i+2] == '\0')
				syntax_error();
			redir_cont2(input, i, '>');
		}
		i++;
	}
}

int	syntax_cont(char *input)
{
	int i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t' || input[i] == '\v'
		|| input[i] == '\f' || input[i] == '\r')
		i++;
	if (input[i] == '\0')
		return (0);
	pipe_cont(input);
	redir_cont(input);
	return (0);
}
