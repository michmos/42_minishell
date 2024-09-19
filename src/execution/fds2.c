/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fds2.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/06 11:50:28 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/19 09:41:58 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (dup2(shell->std_err, STDERR_FILENO) == -1)
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

t_error	input_redirections(int fds[2], t_redir *redir, char *hd_str)
{
	if (redir->type == I_RD_HD)
	{
		if (fds[0] == -2)
			fds[0] = get_hd_fd(hd_str);
	}
	else if (redir->type == I_RD)
	{
		if (close_fd(fds[0]) != NO_ERR)
			return (DEADLY_ERR);
		fds[0] = open(redir->filename, O_RDONLY);
	}
	return (NO_ERR);
}

t_error	output_redirections(int fds[2], t_redir *redir)
{
	if (close_fd(fds[1]) != NO_ERR)
		return (DEADLY_ERR);
	if (redir->type == O_RD)
		fds[1] = open(redir->filename, O_CREAT | O_RDWR | O_TRUNC, 0777);
	else if (redir->type == O_RD_APP)
		fds[1] = open(redir->filename, O_CREAT | O_RDWR | O_APPEND, 0777);
	return (NO_ERR);
}

t_error	handle_redirection_err(int fds[2], t_redir *redir)
{
	if (fds[0] == -1 || fds[1] == -1)
	{
		if (close_fd(fds[0]) != NO_ERR)
			return (DEADLY_ERR);
		if (close_fd(fds[1]) != NO_ERR)
			return (DEADLY_ERR);
		ft_printf_fd(STDERR_FILENO, "%s: %s: %s\n", SHELLNAME, \
		redir->filename, strerror(errno));
		return (ERR);
	}
	return (NO_ERR);
}
