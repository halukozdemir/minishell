#include "../minishell.h"

char	**expand_args(char **args, int old_size, int new_size)
{
	char	**new_args;
	int		i;

	new_args = (char **)malloc(sizeof(char *) * new_size);
	if (!new_args)
		return (NULL);
	// Eski diziyi yeni diziye kopyala
	i = 0;
	while (i < old_size)
	{
		new_args[i] = args[i]; // Elemanları kopyala
		i++;
	}
	free(args);        // Eski diziyi serbest bırak (çünkü realloc yok)
	return (new_args); // Genişletilmiş diziyi geri döndür
}
t_command *create_command(char **tokens)
{
    t_command   *cmd;
    int         i;
    int         arg_size;  // args dizisinin başlangıç boyutu
    int         arg_count; // Eklenen argüman sayısı

    cmd = (t_command *)malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);

    arg_size = 2; // Başlangıçta 2 elemanlık yer ayır
    cmd->args = (char **)malloc(sizeof(char *) * arg_size);
    if (!cmd->args)
        return (NULL);

    arg_count = 0; // Başlangıçta hiç argüman yok
    cmd->input_redirect = NULL;
    cmd->output_redirect = NULL;
    cmd->is_pipe = false;
    cmd->append_mode = false;
    cmd->next = NULL;

    i = 0;
    while (tokens[i])
    {
        // Belleği genişletme gereksinimi
        if (arg_count >= arg_size)
        {
            arg_size *= 2;
            char **new_args = (char **)malloc(sizeof(char *) * arg_size);
            if (!new_args)
                return NULL;

            // Mevcut argümanları yeni dizisine kopyala
            int j = 0;
            while (j < arg_count)
            {
                new_args[j] = cmd->args[j];
                j++;
            }
            free(cmd->args); // Eski belleği serbest bırak
            cmd->args = new_args;
        }

        // Bellek tahsis et ve string'i manuel kopyala
        int len = ft_strlen(tokens[i]);
        printf("Token[%d]: %s (Length: %d)\n", i, tokens[i], len); // Debugging

        // Eğer tokens[i] boş değilse
        if (len > 0)
        {
            cmd->args[arg_count] = (char *)malloc(len + 1);  // +1, null terminatör için
            if (!cmd->args[arg_count])
                return NULL; // Bellek ayırılamazsa, geri dön

            // String kopyala
            ft_memcpy(cmd->args[arg_count], tokens[i], len);
            cmd->args[arg_count][len] = '\0'; // Null terminatör ekle

            printf("args[%d]: %s (from tokens[%d])\n", arg_count, cmd->args[arg_count], i); // Debugging
        }
        else
        {
            printf("Skipping empty token for args[%d]\n", arg_count);  // Eğer token boşsa yazdır
        }

        arg_count++;
        i++;
    }

    // args dizisinin sonuna NULL ekle
    cmd->args[arg_count] = NULL;

    return cmd;
}
