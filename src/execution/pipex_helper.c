/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_helper.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 13:32:12 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/17 14:09:39 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_data	*get_cmd(t_list *lst)
{
	return ((t_cmd_data *)(lst->as_ptr));
}

t_redir	*get_redir(t_list *lst)
{
	return ((t_redir *)(lst->as_ptr));
}

void	err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	close_pipes(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_cmd - 1)
	{
		close(info->fd[i][0]);
		close(info->fd[i][1]);
		i++;
	}
}

void	close_fd_array(t_cmd_data *cmd, t_info *info)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->fd_array[i] > 0 && close(cmd->fd_array[i]) == -1)
			error(ERR_CLOSE, info);
		i++;
	}
}
