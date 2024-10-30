#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// Özel strtok işlevi
char *custom_strtok(char *str, const char *delim)
{
    static char *last;
    char *token;
    int i;

    if (str)
        last = str;
    if (!last || *last == '\0')
        return NULL;

    token = last;
    while (*last)
    {
        i = 0;
        while (delim[i] != '\0')
        {
            if (*last == delim[i])
            {
                *last = '\0';
                last++;
                if (*token != '\0')
                    return token;
                else
                    token = last;
            }
            i++;
        }
        last++;
    }

    last = NULL;
    return *token ? token : NULL;
}

// Komut yolunu bulma fonksiyonu
char *get_command_path(char *cmd) {
    if (access(cmd, X_OK) == 0) {
        return cmd;
    } else {
        char *path = getenv("PATH");
        if (!path) {
            return cmd;
        }
        char *token = custom_strtok(path, ":");
        while (token != NULL) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);
            if (access(full_path, X_OK) == 0) {
                return strdup(full_path);
            }
            token = custom_strtok(NULL, ":");
        }
    }
    return cmd;
}

// Komutu hazırlayan fonksiyon
t_cmd *prepare_command(char *input, t_env *env_list)
{
    t_cmd *shell_cmd;
    char **tokens;
    int token_count = 0;
    int i = 0;
    char *temp_token;

    // İlk geçişte token sayısını belirliyoruz
    char *temp_input = strdup(input);
    temp_token = custom_strtok(temp_input, " ");
    while (temp_token != NULL) {
        token_count++;
        temp_token = custom_strtok(NULL, " ");
    }
    free(temp_input);

    // t_cmd ve executor yapısını oluşturuyoruz
    shell_cmd = malloc(sizeof(t_cmd));
    if (!shell_cmd)
        return NULL;
    shell_cmd->env = env_list;
    shell_cmd->executor = malloc(sizeof(t_executor));
    if (!shell_cmd->executor) {
        free(shell_cmd);
        return NULL;
    }

    // Token sayısı kadar yer açıyoruz
    tokens = malloc(sizeof(char *) * (token_count + 1)); // +1, NULL bitiş için
    temp_token = custom_strtok(input, " ");
    while (temp_token != NULL) {
        tokens[i] = strdup(temp_token);
        i++;
        temp_token = custom_strtok(NULL, " ");
    }
    tokens[i] = NULL;

    // Yönlendirme işlemlerini kontrol et
    shell_cmd->ncmd = tokens;
    shell_cmd->executor->argv = malloc(sizeof(char *) * (token_count + 1)); // Argümanları saklamak için token_count kadar yer açıyoruz
    shell_cmd->executor->redirect = NULL;
    i = 0;
    int arg_count = 0;

    while (tokens[i] != NULL) {
        // ">>" kontrolünü ilk sıraya aldık
        if ((ft_strncmp(tokens[i], ">>", 2) == 0) && tokens[i + 1] != NULL)
        {
            // Append yönlendirme
            t_redirection *new_redir = malloc(sizeof(t_redirection));
            new_redir->type = 11; // Append type için 11 kullanıyoruz
            new_redir->filename = strdup(tokens[i + 1]); // Dosya adını yönlendirmeye ekle
            new_redir->next = shell_cmd->executor->redirect;
            shell_cmd->executor->redirect = new_redir;
            i += 2;
        }
        else if (ft_strncmp(tokens[i], ">", 1) == 0 && tokens[i + 1] != NULL)
        {
            // Üzerine yazma yönlendirme
            t_redirection *new_redir = malloc(sizeof(t_redirection));
            new_redir->type = 13;
            new_redir->filename = strdup(tokens[i + 1]);
            new_redir->next = shell_cmd->executor->redirect;
            shell_cmd->executor->redirect = new_redir;
            i += 2;
        }
        else
        {
            shell_cmd->executor->argv[arg_count] = strdup(tokens[i]);
            arg_count++;
            i++;
        }
    }
    shell_cmd->executor->argv[arg_count] = NULL;

    return shell_cmd;
}

// Bellek temizleme işlemi
void cleanup_command(t_cmd *shell_cmd)
{
    int i = 0;

    if (!shell_cmd)
        return;
    if (shell_cmd->ncmd)
    {
        while (shell_cmd->ncmd[i])
        {
            free(shell_cmd->ncmd[i]);
            i++;
        }
        free(shell_cmd->ncmd);
    }
    if (shell_cmd->executor)
        free(shell_cmd->executor);
    free(shell_cmd);
}

int execute_command(t_cmd *shell_cmd) {
    pid_t pid;
    int status;
    int fd;
    t_executor *executor = shell_cmd->executor;
    t_redirection *redir = executor->redirect;

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Yönlendirmeleri çocuk süreçte gerçekleştiriyoruz
        while (redir) {
            if (redir->type == 13) { // output redirection (üzerine yazma)
                fprintf(stderr, "Redirecting with > to file: %s\n", redir->filename);
                fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            } else if (redir->type == 11) { // append redirection (sonuna ekleme)
                fprintf(stderr, "Redirecting with >> to file: %s\n", redir->filename);
                fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                fprintf(stderr, "Unsupported redirection type\n");
                exit(1);
            }

            if (fd < 0) {
                perror("open");
                exit(1);
            }

            // Çıktıyı yönlendiriyoruz
            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2");
                close(fd);
                exit(1);
            }

            // Dosya descriptorünü kapatıyoruz çünkü yönlendirme yapılmıştır
            close(fd);
            // Sonraki yönlendirmeye geçiyoruz
            redir = redir->next;
        }

        // Komutu çalıştır
        fprintf(stderr, "Executing command: %s\n", executor->argv[0]);
        if (execve(get_command_path(executor->argv[0]), executor->argv, shell_cmd->envp) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else {
        // Ebeveyn süreç çocuk sürecin tamamlanmasını bekliyor
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv, char **env)
{
    char *input;
    t_cmd *shell_cmd;
    t_env *env_list;
    char    **cmd;

    (void)argv;
    if (argc != 1)
        return 1;

    // env'i t_env structına dönüştürme
    env_list = envfunc2(env);

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;
        add_history(input);
        get_dollar(&input, env_list);
        cmd = get_token(input);
        int g = 0;
        while (cmd[g])
        {
            printf("%s\n", cmd[g]);
            g++;
        }
        free(input);
    }

    return 0;
}
