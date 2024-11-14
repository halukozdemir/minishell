#include "../minishell.h"

void	lstadd_back2(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
}

char	*funckey(char *env, int end)
{
	char	*key;
	int		i;

	key = (char *)malloc(end + 2); // Null karakter için ek 1 yer ayrılmalı
	if (!key)
		return (NULL);
	i = 0;
	while (i <= end)
	{
		key[i] = env[i];
		i++;
	}
	key[i] = '\0'; // Stringin sonuna null karakteri eklenmeli
	return (key);
}

char	*funcval(char *env, int start)
{
	char	*val;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (env[start + len]) // Değerin uzunluğunu hesapla
		len++;
	val = (char *)malloc(len + 1); // Null karakter için ek yer
	if (!val)
		return (NULL);
	while (env[start])
	{
		val[i] = env[start];
		start++;
		i++;
	}
	val[i] = '\0'; // String sonlandırıcıyı doğru yerde ekle
	return (val);
}

t_env	*envfunc2(char **env)
{
	int		i;
	int		end;
	t_env	*new;
	t_env	*lst;

	lst = NULL; // Liste başlangıçta boş olmalı
	i = 0;
	while (env[i])
	{
		end = 0;
		while (env[i][end] && env[i][end] != '=') // Anahtarın sonuna kadar ilerle
			end++;
		new = (t_env *)malloc(sizeof(t_env));
		if (!new)
			return (NULL);
		new->key = funckey(env[i], end - 1); // '=' işaretinden önceki kısım anahtar
		if (!new->key)
			return (NULL);
		new->value = funcval(env[i], end + 1); // '=' işaretinden sonrası değer
		if (!new->value)
			return (NULL);
		new->next = NULL;
		new->prev = NULL;
		lstadd_back2(&lst, new); // Yeni düğümü listeye ekle
		i++;
	}
	return (lst);
}
