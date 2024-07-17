/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:16:48 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/17 11:17:27 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	get_hd_fd(t_cmd *cmd, t_info *info)
{
	int	fd[2];

	if (pipe(fd) == -1)
		// TODO: Error message
	write(fd[1], cmd->hd_str, ft_strlen(cmd->hd_str));
	close(fd[1]);
	cmd->fd_array[cmd->last_input] = fd[0];
}

void	heredoc(t_cmd *cmd, t_info *info, int i)
{
	if (cmd->last_input == i)
		get_hd_fd(cmd, info);
	else
		cmd->fd_array[i] = -2;
}

/*
    basically we are checking if heredoc is the last input
    if it is we'll get the heredoc string and print it to 
    the terminal
    if not we asign -2 and be a check that we don't want to print it
*/