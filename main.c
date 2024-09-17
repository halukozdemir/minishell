#include "minishell.h"

int main(int argc, char **argv, char **env)
{
    t_cmd   *cmd;

    (void)argv;
    if (argc != 1)
    {
        printf("Argument error. \n");
        exit(1);
    }
    cmd = (t_cmd *)malloc(sizeof(t_cmd));
    if (!cmd)
        return (0);
    init_struct(cmd);
}
