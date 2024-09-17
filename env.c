/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beyildiz <beyildiz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 15:45:07 by beyildiz          #+#    #+#             */
/*   Updated: 2024/09/17 16:25:22 by beyildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*funckey(char	*env, int end)
{
	char *key;

	key = (char *)malloc((end) * (sizeof(char) + 1));
	key[end + 1] = '\0';
	while(end != 0)
	{
		key[end] = env[end];
		end--;
	}
	return(key);
}

char	*funcval(char	*env, int start)
{
	char	*val;
	int 	end = 0;
	while(env[start])
	{
		end++;
	}
	end--;
	val = (char *)malloc(end - start + 1);
	while(env[start])
	{
		val[start] = env[start];
		start++;
	}
	val[start] = '\0';
	return(val);
}

void	env(char	**env)
{
	int i;
	int end;
	

	i = 0;
	end = 0;
	while(env[i])
	{
		while(env[end] != '=')
		{
			end++;
		}
		funckey(env[i], end - 1);
		funcval(env[i], end + 1);
		i++;
	}
}
