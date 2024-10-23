#include "minishell.h"

void free_command_list(t_command *cmd)
{
    t_command *tmp;

    while (cmd)
    {
        tmp = cmd->next;

        // Argümanları serbest bırak
        int i = 0;
        if (cmd->args)
        {
            while (cmd->args[i])
            {
                if (cmd->args[i])  // Serbest bırakmadan önce boş olup olmadığını kontrol et
                {
                    printf("Freeing args[%d]: %s\n", i, cmd->args[i]);
                    free(cmd->args[i]);
                }
                i++;
            }
            free(cmd->args);
        }

        // Input redirect ve output redirect işlemleri (varsa)
        if (cmd->input_redirect)
        {
            if (cmd->input_redirect->filename)
                free(cmd->input_redirect->filename);
            free(cmd->input_redirect);
        }

        if (cmd->output_redirect)
        {
            if (cmd->output_redirect->filename)
                free(cmd->output_redirect->filename);
            free(cmd->output_redirect);
        }

        // Komut yapısını serbest bırak
        free(cmd);
        cmd = tmp;
    }
}

void print_command(t_command *cmd)
{
    int i = 0;
    printf("Command and arguments:\n");
    while (cmd->args && cmd->args[i])
    {
        printf("arg[%d]: %s\n", i, cmd->args[i]);
        i++;
    }

    if (cmd->input_redirect)
        printf("Input redirect: %s\n", cmd->input_redirect->filename);
    else
        printf("Input redirect: None\n");

    if (cmd->output_redirect)
        printf("Output redirect: %s\n", cmd->output_redirect->filename);
    else
        printf("Output redirect: None\n");

    printf("This command is %spiped.\n", cmd->is_pipe ? "" : "not ");
}

int main(int argc, char **argv, char **env)
{
    char *input;
    char **tokens;
    t_env *env_list;
    t_command *cmd_list;
    bool has_error;

    (void)argv; // Argümanları kullanmıyoruz
    if (argc != 1)
    {
        printf("Argument error.\n");
        return 1;
    }

    // Env değişkenlerini struct'a kaydediyoruz
    env_list = envfunc2(env);

    printf("-----------\n");

    while (1)
    {
        // Kullanıcı girdisini al
        input = readline("minishell> ");
        if (!input) // CTRL+D veya EOF sinyali
            break;

        add_history(input); // Girilen komutu history'ye ekle

        // Syntax kontrolü yap
        has_error = false;
        // if (!syntax_cont(input, &has_error) && has_error)
        // {
        //     printf("Syntax error.\n");
        //     free(input);
        //     continue;
        // }
		get_dollar(&input, env_list);
		printf("yeni str: \"%s\"\n", input);
        // Girdiyi token'lara ayır (parser işlemi)
        tokens = get_token(input);

        // get_token'dan dönen tokenları struct yapısına dönüştür
        // cmd_list = create_command(tokens); // Komutları struct'a dönüştürüyoruz

        // Komutları yazdırarak test ediyoruz
        // print_command(cmd_list);

        // Free işlemleri (tokens ve cmd_list)
        free(input); // Kullanıcı girdisini serbest bırak

        // Tokens dizisini serbest bırak
        int i = 0;
        while (tokens[i])
        {
            free(tokens[i]);
            i++;
        }
        free(tokens);

        // Cmd_list'teki tüm komutları serbest bırak (recursive olarak)
        free_command_list(cmd_list);
    }

    return 0;
}
