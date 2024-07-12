/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_open_files.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/11 09:48:26 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/11 11:29:06 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_append(t_cmd *cmd, t_info *info, int i)
{
	cmd->fd_array[i] = open(((t_redir *)(cmd->redir_lst))->filename, \
	O_CREAT | O_RDWR | O_APPEND, 0777);
	if (cmd->fd_array[i] == -1)
		//TODO:error opening file
	return (0);
}

int	open_input_output(t_cmd *cmd, t_info *info, int i)
{
	if (((t_redir *)(cmd->redir_lst))->type == I_RD)
	{
		cmd->fd_array[i] = open(((t_redir *)(cmd->redir_lst))->filename, \
		O_RDONLY);
		if (cmd->fd_array[i] == -1)
		{
			//TODO:error opening file
		}
	}
	else if (((t_redir *)(cmd->redir_lst))->type == O_RD)
	{
		cmd->fd_array[i] = open(((t_redir *)(cmd->redir_lst))->filename, \
		O_CREAT | O_RDWR | O_TRUNC, 0777);
		if (cmd->fd_array[i] == -1)
		{
			//TODO:error opening file
		}
	}
	return (0);
}

int	open_one_file(t_cmd *cmd, t_info *info, int i)
{
	int		status;
	t_tag	*type;

	status = 0;
	type = ((t_redir *)(cmd->redir_lst))->type;
	if (type == I_RD || type == O_RD)
		status = open_input_output(cmd, info, i);
	else if (type == O_RD_APP)
		status = open_append(cmd, info, i);
	else if (type == I_RD_HD)
		heredoc(cmd, info, i); //TODO
	return (status);
}

void	open_files(t_cmd *cmd, t_info *info)
{
	int		i;
	int		status;
	t_cmd	*temp;

	cmd->redir_count = ft_lstsize(temp->redir_lst);
	if (cmd->redir_count == 0)
		return (0);
	cmd->fd_array = (int *)malloc((cmd->redir_count * sizeof(int)) + 1);
	i = 0;
	while (i < cmd->redir_count)
	{
		status = open_one_file(cmd, info, i);
		if (status == 1)
			return (1);
		cmd->redir_lst = cmd->redir_lst->next;
		i++;
	}
	return (0);
}
