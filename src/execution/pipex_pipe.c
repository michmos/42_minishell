/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/11 11:39:11 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/18 14:15:07 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	dup2_copy(int old_fd, int new_fd, t_info *info)
{
	if (dup2(old_fd, new_fd) == -1)
		error(ERR_DUP, info);
}

void	pipe_cmd(t_cmd_data *cmd, t_info *info, int i)
{
	if (cmd->last_input == -1 && i > 0)
		dup2_copy(info->fd[i - 1][0], STDIN_FILENO, info);
	else if (cmd->last_input > -1)
		dup2_copy(cmd->fd_array[cmd->last_input], STDIN_FILENO, info);
	if (cmd->last_output == -1 && i != info->num_cmd - 1)
		dup2_copy(info->fd[i][1], STDOUT_FILENO, info);
	else if (cmd->last_output > -1)
		dup2_copy(cmd->fd_array[cmd->last_output], STDOUT_FILENO, info);
}

/*
	1. no input && not first command
	2. if we have input, read from it
	3. if no output && not last command
	4. if there is output, write result to it
*/
