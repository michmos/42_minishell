/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_helper.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 13:32:12 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/03 09:50:22 by pminialg      ########   odam.nl         */
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

int	i_o_redirection(int input_fd, int output_fd)
{
	if (dup2(input_fd, STDIN_FILENO) == -1)
		return (ERROR);
	if (input_fd != STDIN_FILENO)
	{
		if (close(input_fd) == -1)
			return (ERROR);
	}
	if (dup2(output_fd, STDOUT_FILENO) == -1)
		return (ERROR);
	if (output_fd != STDOUT_FILENO)
	{
		if (close(output_fd) == -1)
			return (ERROR);
	}
	return (0);
}
