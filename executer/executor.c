
// #include "../minishell.h"

// static void	setup_pipes(int pipes[][2], int noj)
// {
// 	int	i;

// 	i = -1;
// 	while (++i + 1 < noj)
// 	{
// 		if (pipe(pipes[i]) == -1)
// 		{
// 			perror("pipe");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// }

// static void	close_all_pipes(int pipes[][2], int noj)
// {
// 	int	i;

// 	i = -1;
// 	while (++i + 1 < noj)
// 	{
// 		close(pipes[i][0]);
// 		close(pipes[i][1]);
// 	}
// }

// static void	exec_child(int i, t_mshell *mshell, int pipes[][2], char **envp)
// {
// 	if (i > 0)
// 		dup2(pipes[i - 1][0], STDIN_FILENO);
// 	if (i + 1 < mshell->jobs->len)
// 		dup2(pipes[i][1], STDOUT_FILENO);
// 	close_all_pipes(pipes, mshell->jobs->len);
//     // while içine al, next ile struct'tan sonraki job'lara giderek execve'ye gönder
// 	execve(mshell->success_arr[i], mshell->jobs->job_list->args, envp);
//     //
// 	perror("execve");
// }

// char	executor(t_mshell *mshell)
// {
//     // her şey bittikten sonra pipes[2] yapmaya çalışabilirsin
// 	int		pipes[mshell->jobs->len - 1][2];
// 	pid_t	pid;
// 	int		i;

// 	mshell->success_arr = accessor(mshell->jobs);
// 	setup_pipes(pipes, mshell->jobs->len);
// 	i = -1;
// 	while (++i < mshell->jobs->len)
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 			return(perror("fork"), 1);
// 		if (pid == 0)
// 			exec_child(i, mshell->jobs, pipes, mshell->env);
// 	}
// 	close_all_pipes(pipes, mshell->jobs->len);
// 	i = -1;
// 	while (++i < mshell->jobs->len)
// 		wait(NULL);
// 	return (0);
// }

// // parser'dan gelen char **'da pipe gördüğünde job->next_job'a yer ayır,
// // bu adrese gel, cmd ve args'ları doldurmaya devam et
