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

void get_hd_str(t_info *info, t_cmd_data *cmd)
{
	/*
		i want to get the input either with readline or gnl
		there can be multiple heredocs, so my while loop
		needs to take that into account
	*/
}

void init_heredoc(t_info *info, t_cmd_data *cmd)
{
	int i;

	i = 0;
	cmd->hd_array = malloc((cmd->hd_count + 1) * sizeof(char *));
	if (cmd->hd_array == NULL)
		return ;
	while (i < cmd->redir_count)
	{
		/*
			i want to malloc here if the redirection type is I_RD_HD
			but how much do i wan to malloc to put it in the array???
		*/
		i++;
	}
	get_hd_str(info, cmd);
}

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