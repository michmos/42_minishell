/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fds2.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/06 11:50:28 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/11 10:35:04 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

t_error	reset_io(void)
{
	t_shell	*shell;

	shell = get_shell_struct();
	if (dup2(shell->std_in, STDIN_FILENO) == -1)
	{
		perror("dup2");
		return (DEADLY_ERR);
	}
	if (dup2(shell->std_out, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		return (DEADLY_ERR);
	}
	return (0);
}

t_error	redir(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		perror("dup2");
		return (DEADLY_ERR);
	}
	if (close(old_fd) == -1)
	{
		perror("close");
		return (DEADLY_ERR);
	}
	return (NO_ERR);
}
