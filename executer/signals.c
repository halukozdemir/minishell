#include "../minishell.h"

static void	ctrl_output(t_mshell *mshell, char state)
{
	if (state)
	{
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &mshell->termios);
	}
}
