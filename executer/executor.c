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
    int         arg_count;
    int         i;

    cmd = (t_command *)malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);

    // args için başlangıçta 3 elemanlık bir yer ayırıyoruz
    cmd->args = (char **)malloc(sizeof(char *) * 3);
    if (!cmd->args)
        return (NULL);

    // Input ve output redirect ve diğer yapıları sıfırlıyoruz
    cmd->input_redirect = NULL;
    cmd->output_redirect = NULL;
    cmd->is_pipe = false;
    cmd->append_mode = false;
    cmd->next = NULL;

    arg_count = 0;
    i = 0;
    // İlk argümanı manuel olarak işliyoruz
    while (tokens[arg_count])
    {
        cmd->args[arg_count] = ft_strdup(tokens[arg_count]);
        printf("args[%d]: %s\n", arg_count, cmd->args[arg_count]);
        arg_count++;
    }
    // args dizisinin sonuna NULL ekliyoruz
    cmd->args[arg_count] = NULL;
    printf("Added NULL to args[%d]\n", arg_count);  // Debugging

    return cmd;
}
