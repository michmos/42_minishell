/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_open_files.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/11 09:48:26 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/15 09:27:09 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	open_append(t_cmd_data *cmd, t_info *info, int i, int proc)
// {
// 	cmd->fd_array[i] = open(((t_redir *)(cmd->pars_out->redir_lst))->filename,
// 			O_CREAT | O_RDWR | O_APPEND, 0777);
// 	if (cmd->fd_array[i] == -1)
// 		if (error_open(cmd, proc, info))
// 			return (1);
// 	return (0);
// }

// int	open_input_output(t_cmd_data *cmd, t_info *info, int i, int proc)
// {
// 	if (((t_redir *)(cmd->pars_out->redir_lst))->type == I_RD)
// 	{
// 		cmd->fd_array[i] = open(((t_redir *)(cmd->pars_out->redir_lst))->filename,
// 				O_RDONLY);
// 		if (cmd->fd_array[i] == -1)
// 			if (error_open(cmd, proc, info))
// 				return (1);
// 	}
// 	else if (((t_redir *)(cmd->pars_out->redir_lst))->type == O_RD)
// 	{
// 		cmd->fd_array[i] = open(((t_redir *)(cmd->pars_out->redir_lst))->filename,
// 				O_CREAT | O_RDWR | O_TRUNC, 0777);
// 		if (cmd->fd_array[i] == -1)
// 			if (error_open(cmd, proc, info))
// 				return (1);
// 	}
// 	return (0);
// }

static int	open_one_file(t_redir *redir)
{
	int		fd;
	t_tag	type;

	// (void)process; // TODO: see where i use it, or if i need it here
	type = redir->type;
	if (type == I_RD)
		fd = open(redir->filename, O_RDONLY);
	else if (type == O_RD)
		fd = open(redir->filename, O_CREAT | O_RDWR | O_TRUNC, 0777);
	else if (type == O_RD_APP)
		fd = open(redir->filename, O_CREAT | O_RDWR | O_APPEND, 0777);
	// else if (type == I_RD_HD)
	// 	heredoc(cmd, info, i);
	return (fd);
}

t_error	open_files(int *fd_array, t_list *redir_lst)
{
	int	i;
	int	status;

	i = 0;
	while (redir_lst)
	{
		fd_array[i] = open_one_file((t_redir *)(redir_lst->as_ptr));
		if (fd_array[i] == -1)
		{
			perror("open");
			if (close_fd_array(fd_array, i) != NO_ERR)
			{
				return (SYS_ERR);
			}
			return (SYS_ERR);
		}
		redir_lst = redir_lst->next;
		i++;
	}
	return (0);
}
