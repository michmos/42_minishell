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

t_error	close_fd(int fd)
{
	if (fd >= 0)
	{
		if (close(fd) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}
	}
	return (NO_ERR);
}
