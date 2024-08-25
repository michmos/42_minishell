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

int	open_one_file(t_cmd_data *cmd, int process, t_info *info, int i)
{
	int		fd;
	t_tag	type;

	(void)process; // TODO: see where i use it, or if i need it here
	type = ((t_redir *)(cmd->pars_out->redir_lst))->type;
	if (type == I_RD)
		fd = open(((t_redir *)(cmd->pars_out->redir_lst))->filename, O_RDONLY);
	else if (type == O_RD)
		fd = open(((t_redir *)(cmd->pars_out->redir_lst))->filename, O_CREAT | O_RDWR | O_TRUNC, 0777);
	else if (type == O_RD_APP)
		fd = open(((t_redir *)(cmd->pars_out->redir_lst))->filename, O_CREAT | O_RDWR | O_APPEND, 0777);
	else if (type == I_RD_HD)
		heredoc(cmd, info, i);
	return (fd);
}

int	open_files(t_cmd_data *cmd, int process, t_info *info)
{
	int			i;
	int			status;
	t_cmd_data	*temp;

	temp = cmd;
	cmd->redir_count = ft_lstsize(temp->pars_out->redir_lst);
	if (cmd->redir_count == 0)
		return (0);
	cmd->fd_array = (int *)malloc((cmd->redir_count * sizeof(int)) + 1);
	if (cmd->fd_array == NULL)
		return (ERROR);
	i = 0;
	while (i < cmd->redir_count)
	{
		cmd->fd_array[i] = open_one_file(cmd, process, info, i);
		if (cmd->fd_array[i] == -1)
			return (1); // TODO: error opening file, will put an error function here probably can use this one -> (error_open(cmd, proc, info)), but lets see
		cmd->pars_out->redir_lst = cmd->pars_out->redir_lst->next;
		i++;
	}
	return (0);
}
