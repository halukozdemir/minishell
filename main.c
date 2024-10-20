#include "minishell.h"
#include <string.h>

int main(int argc, char **argv, char **env)
{
	char	*input;
	char	**cmds;
	bool	has_error;
	t_env	*env_list;

	(void)argv;
	if (argc != 1)
	{
		printf("Argument error.\n");
		exit(1);
	}

	// Env değişkenlerini işleyip struct'a kaydediyoruz
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
		if (!syntax_cont(input, &has_error) && has_error)
		{
			printf("Syntax error.\n");
			free(input);
			continue; // Eğer syntax hatası varsa yeni girdi al
		}

		// Girdiyi token'lara ayır (parser işlemi)
		cmds = get_token(input);

		// $ işaretinden sonraki env değişkenlerini genişlet
		get_dollar(&input, env_list);

		// Executor'u çağırarak komutları çalıştır
		executor(cmds, env);

		// Belleği temizle
		free(input);
		// Free cmds, token'ları temizle
		int i = 0;
		while (cmds[i])
		{
			free(cmds[i]);
			i++;
		}
		free(cmds);
	}
	return (0);
}
