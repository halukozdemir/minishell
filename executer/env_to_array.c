#include "../minishell.h"

char    **env_to_array(t_env *lst)
{
    char    **env_array;
    t_env   *temp;
    int     count;
    int     i;

    // Listenin eleman sayısını bul
    count = 0;
    temp = lst;
    while (temp)
    {
        count++;
        temp = temp->next;
    }

    // Char ** için yer ayır
    env_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);

    // Env structının elemanlarını char double pointer'a yaz
    i = 0;
    temp = lst;
    while (temp)
    {
        env_array[i] = (char *)malloc(ft_strlen(temp->key) + ft_strlen(temp->value) + 2); // '=' ve null karakteri için ek alan
        if (!env_array[i])
        {
            // Hata durumunda, daha önce ayrılmış belleği temizle
            while (i > 0)
                free(env_array[--i]);
            free(env_array);
            return (NULL);
        }
        ft_strlcpy(env_array[i], temp->key, ft_strlen(temp->key) + 1);
        ft_strlcat(env_array[i], "=", ft_strlen(temp->key) + 2);
        ft_strlcat(env_array[i], temp->value, ft_strlen(temp->key) + ft_strlen(temp->value) + 2);
        i++;
        temp = temp->next;
    }
    env_array[i] = NULL; // Diziyi sonlandır
    return (env_array);
}
