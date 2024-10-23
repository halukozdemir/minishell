#include "../minishell.h"

void	check_quotes(char c, bool *sq, bool *dq)
{
	if (c == '\'' && !*dq)
		*sq = !*sq;
	if (c == '\"' && !*sq)
		*dq = !*dq;
}

void	replace_dollar_with_value_or_remove(char **input, char *key,
			char *value, int start, int end, bool needs_quotes)
{
	char	*new_input;
	int		new_len;

	// Eğer value varsa, value'yi yerleştir; yoksa key'i tamamen kaldır
	if (value)
		new_len = ft_strlen(*input) + ft_strlen(value) - (end - start);
	else
		new_len = ft_strlen(*input) - (end - start);  // Geçersiz değişkeni tamamen kaldır

	new_input = (char *)malloc(new_len + 1);
	if (!new_input)
		return ;
	ft_strlcpy(new_input, *input, start + 1); // Başlangıç kısmını kopyala

	// Eğer value varsa, value'yi yerleştir
	if (value)
		ft_strlcat(new_input, value, new_len + 1);

	// Kalan kısmı ekle
	ft_strlcat(new_input, *input + end, new_len + 1);
	free(*input);
	*input = new_input;
}


char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0
			&& ft_strlen(env->key) == ft_strlen(key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

bool contains_special_operators(char *key)
{
    // Özel operatörleri kontrol et (<, >, <<, >>)
    if (ft_strnstr(key, "<", ft_strlen(key)) ||
        ft_strnstr(key, ">", ft_strlen(key)) ||
        ft_strnstr(key, ">>", ft_strlen(key)) ||
        ft_strnstr(key, "<<", ft_strlen(key)))
        return (true);
    return (false);
}

void	process_key(char **input_ptr, t_env *env, int *i, bool in_single_quotes)
{
	int		j;
	char	*key;
	char	*value;

	// Eğer tek tırnak içindeysek genişletme yapma
	if (in_single_quotes)
	{
		(*i)++;
		return;
	}

	// $ işaretinden sonra değişkeni al
	j = *i;
	while (ft_isalnum((*input_ptr)[j]) || (*input_ptr)[j] == '_')
		j++;
	key = ft_substr(*input_ptr, *i, j - *i); // $ işaretinden sonra gelen anahtar kelimeyi al
	value = get_env_value(env, key); // Çevresel değişkeni getir

	// Eğer value NULL ise, geçersiz değişkeni tamamen kaldır
	if (!value && ft_strlen(key) > 0)
	{
		replace_dollar_with_value_or_remove(input_ptr, "", NULL, *i - 1, j, false);
		*i = *i - 1;  // Geçersiz kısmı kaldırdıktan sonra indeksi ayarla
	}
	// Eğer value geçerli ise, değişkenin değerini yerine koy
	else if (value)
	{
		replace_dollar_with_value_or_remove(input_ptr, key, value, *i - 1, j, false);
		*i = *i + ft_strlen(value) - 1;  // Değiştirilen değer için indeksi ayarla
	}

	free(key);
}

void    get_dollar(char **input_ptr, t_env *env)
{
    char *input;
    char *new_input;
    int i = 0, j;
    int len = 0;
    int total_len = 0;
    bool in_single_quotes = false; // Tek tırnak içinde olup olmadığımızı izlemek için
    bool in_double_quotes = false; // Çift tırnak içinde olup olmadığımızı izlemek için

    input = *input_ptr;

    // 1. Toplam uzunluğu hesapla
    while (input[i])
    {//$HOME*GJ
        check_quotes(input[i], &in_single_quotes, &in_double_quotes); // Tırnak durumlarını kontrol et
        if (input[i] == '$' && !in_single_quotes) // Sadece tek tırnak içinde değilsek genişletme yap
        {
            i++;
            j = i;
            while (ft_isalnum(input[j]) || input[j] == '_')
                j++;

            char *key = ft_substr(input, i, j - i); // Değişken ismi al
            char *value = get_env_value(env, key); // Değeri bul
            if (value)
                total_len += ft_strlen(value);  // Değer uzunluğunu ekle
            free(key);
            i = j; // Geçersiz veya geçerli kısmı atlarken devam et
        }
        else
        {
            total_len++;  // Sabit karakterler için toplam uzunluğu artır
            i++;
        }
    }
    // 2. Bellekte yeni string için yer ayır
    new_input = (char *)malloc(total_len + 1);
    if (!new_input)
        return;

    i = 0;
    len = 0;

    // 3. Yeni stringi oluştur
    while (input[i])
    {
        check_quotes(input[i], &in_single_quotes, &in_double_quotes); // Tırnak durumlarını kontrol et
        if (input[i] == '$' && !in_single_quotes) // Sadece tek tırnak içinde değilsek genişletme yap
        {
            i++;
            j = i;
            while (ft_isalnum(input[j]) || input[j] == '_')
                j++;

            char *key = ft_substr(input, i, j - i); // Değişken ismi al
            char *value = get_env_value(env, key); // Değerini bul
            free(key);
            if (value)
            {
                ft_strlcpy(new_input + len, value, ft_strlen(value) + 1); // Değeri ekle
                len += ft_strlen(value);  // Değer uzunluğunu güncelle
            }
            i = j;
        }
        else
        {
            new_input[len++] = input[i++];  // Sabit karakterleri ekle
        }
    }
    new_input[len] = '\0';
    free(*input_ptr);
    *input_ptr = new_input;
}
