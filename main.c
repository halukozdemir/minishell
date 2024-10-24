#include "minishell.h"

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

// cat  "ljfj f"ahg < txt | wc -l |
/*
cmd: cat
args[0] : cat
args[1] :"ljfj f"
args[2] : ahg

next;

cmd: wc
args[0] : wc
args[1] : -l


*/
