/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_helper.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 13:32:12 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/16 15:03:34 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*get_cmd(t_list *lst)
{

	return ((t_cmd *)(lst->as_ptr));
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

t_error	close_fd_array(int *fd_array, size_t size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (fd_array[i] > 0 && close(fd_array[i]) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}
		i++;
	}
	return (NO_ERR);
}
