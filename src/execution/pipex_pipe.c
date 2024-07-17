/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_pipe.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/11 11:39:11 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/17 14:03:34 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_cmd(t_cmd_data *cmd, t_info *info, int i)
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

/*
	i need error checking in case dup2 fails
	so will have to create a helper function 
	that would perform dup2 and take care of the
	error messages
*/
