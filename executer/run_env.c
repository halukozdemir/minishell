#include "../minishell.h"
#include <stdlib.h>
#include <string.h>

int count_env_length(t_env *lst)
{
    int count = 0;
    while (lst)
    {
        count++;
        lst = lst->next;
    }
    return (count);
}

char **env_to_char_array(t_env *env_list)
{
    int i = 0;
    int env_count = count_env_length(env_list);
    char **env_array = (char **)malloc(sizeof(char *) * (env_count + 1)); // +1 for NULL termination
    t_env *current = env_list;

    if (!env_array)
        return (NULL);

    while (current)
    {
        int key_len = strlen(current->key);
        int value_len = strlen(current->value);

        // Allocate memory for "key=value" format string
        env_array[i] = (char *)malloc(key_len + value_len + 2); // +2 for '=' and '\0'
        if (!env_array[i])
        {
            // Free previously allocated memory if allocation fails
            while (i > 0)
            {
                free(env_array[--i]);
            }
            free(env_array);
            return (NULL);
        }

        // Combine key and value into "key=value" format
        strcpy(env_array[i], current->key);
        strcat(env_array[i], "=");
        strcat(env_array[i], current->value);

        i++;
        current = current->next;
    }
    env_array[i] = NULL; // Null terminate the array

    return env_array;
}

void free_env_array(char **env_array)
{
    int i = 0;
    while (env_array[i])
    {
        free(env_array[i]);
        i++;
    }
    free(env_array);
}
