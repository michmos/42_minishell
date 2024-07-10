#include "../minishell.h"

void pipe_cmd(t_cmd *cmd, t_info *info, int i)
{
    if (cmd->last_input == -1 && i > 0) // no input && not first command
        dup2(info->fd[i - 1][0], STDIN_FILENO);
    else if (cmd->last_input > -1) // if we have input, read from it
        dup2(cmd->fd_array[cmd->last_input], STDIN_FILENO);
    if (cmd->last_output == -1 && i != info->num_cmd - 1) // if no output && not last command
        dup2(info->fd[i][1], STDOUT_FILENO);
    else if (cmd->last_output > -1) // if there is output, write result to it
        dup2(cmd->fd_array[cmd->last_output], STDOUT_FILENO);
}
