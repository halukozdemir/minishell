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
        env_array[i] = (char *)malloc(strlen(temp->key) + strlen(temp->value) + 2); // '=' ve null karakteri için ek alan
        if (!env_array[i])
        {
            // Hata durumunda, daha önce ayrılmış belleği temizle
            while (i > 0)
                free(env_array[--i]);
            free(env_array);
            return (NULL);
        }
        strcpy(env_array[i], temp->key);
        strcat(env_array[i], "=");
        strcat(env_array[i], temp->value);
        i++;
        temp = temp->next;
    }
    env_array[i] = NULL; // Diziyi sonlandır
    return (env_array);
}
