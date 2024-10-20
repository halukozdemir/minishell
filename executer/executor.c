#include "../minishell.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*find_command_path(char *command, char **env)
{
	char	*path_env;
	char	**paths;
	char	*command_path;
	int		i;
	size_t	path_len;

	// PATH çevre değişkenini bul
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);

	// PATH'i ':' ile ayır ve her bir dizini kontrol et
	paths = ft_split(path_env, ':'); // ft_split, PATH'i ayırır
	i = 0;
	while (paths[i])
	{
		// command_path için yeterli alan ayır
		path_len = strlen(paths[i]) + strlen(command) + 2;
		command_path = malloc(path_len);
		if (!command_path)
			return (NULL);

		// paths[i]'yi ve komutun adını birleştiriyoruz
		ft_strlcpy(command_path, paths[i], path_len);
		ft_strlcat(command_path, "/", path_len);
		ft_strlcat(command_path, command, path_len);

		if (access(command_path, X_OK) == 0) // Komutun çalıştırılabilir olup olmadığını kontrol et
		{
			free(paths);
			return (command_path); // Komut bulundu
		}
		free(command_path);
		i++;
	}
	free(paths);
	return (NULL); // Komut bulunamadıysa
}

void handle_redirection(char *filename, int redir_type)
{
    int filefd;

    if (redir_type == 1) // ">" out redirection
        filefd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (redir_type == 2) // ">>" append redirection
        filefd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (redir_type == 3) // "<" in redirection
        filefd = open(filename, O_RDONLY);

    if (filefd < 0)
    {
        perror("open");
        exit(1);
    }

    // stdout'u veya stdin'i yönlendir
    if (redir_type == 1 || redir_type == 2)
        dup2(filefd, STDOUT_FILENO);
    else if (redir_type == 3)
        dup2(filefd, STDIN_FILENO);

    close(filefd);
}
void	execute_pipe(char **cmd1, char **cmd2, char **env)
{
	int		pipefd[2];
	pid_t	pid1, pid2;
	char	*command_path1;
	char	*command_path2;

	// Pipe oluştur
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}

	// İlk komut için tam yolunu bul
	command_path1 = find_command_path(cmd1[0], env);
	if (!command_path1)
	{
		printf("%s: command not found\n", cmd1[0]);
		return;
	}

	// İkinci komut için tam yolunu bul
	command_path2 = find_command_path(cmd2[0], env);
	if (!command_path2)
	{
		printf("%s: command not found\n", cmd2[0]);
		return;
	}

	// İlk child process: cmd1 (örneğin ls)
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		exit(1);
	}

	if (pid1 == 0) // Child process 1
	{
		close(pipefd[0]); // Pipe'ın okuma ucunu kapat
		dup2(pipefd[1], STDOUT_FILENO); // stdout'u pipe'a yönlendir
		close(pipefd[1]); // Pipe'ın yazma ucunu kapat
		execve(command_path1, cmd1, env); // Komutu çalıştır (örn: /bin/ls)
		perror("execve");
		exit(1);
	}

	// İkinci child process: cmd2 (örneğin grep)
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		exit(1);
	}

	if (pid2 == 0) // Child process 2
	{
		close(pipefd[1]); // Pipe'ın yazma ucunu kapat
		dup2(pipefd[0], STDIN_FILENO); // stdin'i pipe'dan al
		close(pipefd[0]); // Pipe'ın okuma ucunu kapat
		execve(command_path2, cmd2, env); // Komutu çalıştır (örn: /bin/grep)
		perror("execve");
		exit(1);
	}

	// Parent process: Pipe dosya tanımlayıcılarını kapat ve child'ları bekle
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0); // İlk child'ı bekle
	waitpid(pid2, NULL, 0); // İkinci child'ı bekle

	free(command_path1);
	free(command_path2);
}

void	execute_command(char **cmd, char **env)
{
	pid_t	pid;
	char	*command_path;

	// Komutun tam yolunu bul
	command_path = find_command_path(cmd[0], env);
	if (!command_path)
	{
		printf("%s: command not found\n", cmd[0]);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0) // Child process
	{
		// execve ile komutu çalıştır
		execve(command_path, cmd, env);
		perror("execve");
		exit(1);
	}

	// Parent process, child'ı bekler
	waitpid(pid, NULL, 0);
	free(command_path); // Bellekte ayrılan dosya yolunu temizle
}

void executor(char **cmds, char **env)
{
    // Eğer pipe varsa
    if (contains_pipe(cmds))
    {
        // cmds'yi iki komuta böl: "ls" ve "grep minishell"
        char *cmd1[] = {cmds[0], NULL};
        char *cmd2[] = {cmds[2], cmds[3], NULL};

        // Pipe işlemi
        execute_pipe(cmd1, cmd2, env);
    }
    // Eğer redirection varsa
    else if (contains_redirection(cmds))
    {
        // Redirection tipi ve dosya adı
        int redir_type = 1; // Örnek olarak out redirection (">")
        handle_redirection(cmds[1], redir_type);

        // Komutu çalıştır
        execute_command(cmds, env);
    }
    // Normal komut
    else
    {
        // Normal komutları çalıştır
        execute_command(cmds, env);
    }
}


int	contains_pipe(char **cmds)
{
	int i = 0;

	while (cmds[i])
	{
		if (ft_strncmp(cmds[i], "|", 1) == 0)
			return (1); // Pipe buldu
		i++;
	}
	return (0); // Pipe yok
}

int	contains_redirection(char **cmds)
{
	int i = 0;

	while (cmds[i])
	{
		if ((ft_strncmp(cmds[i], ">", 1) == 0 && ft_strlen(cmds[i]) == 1) ||
			(ft_strncmp(cmds[i], ">>", 2) == 0 && ft_strlen(cmds[i]) == 2) ||
			(ft_strncmp(cmds[i], "<", 1) == 0 && ft_strlen(cmds[i]) == 1))
			return (1); // Redirection bulundu
		i++;
	}
	return (0); // Redirection yok
}
