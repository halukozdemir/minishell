#include "minishell.h"

void	init_struct(t_command *cmd)
{
	cmd->args = NULL;
	cmd->next = NULL;
	cmd->str = NULL;
}
