/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fds.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/29 10:02:34 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/18 13:59:29 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

t_error	set_io_pipes(size_t child_i, size_t num_childs)
{
	int		fds[2];
	t_shell	*shell;

	shell = get_shell_struct();
	if (child_i > 0)
	{
		if (redir(shell->cur_cmdline.open_pipe_end, STDIN_FILENO) != NO_ERR)
			return (DEADLY_ERR);
		shell->cur_cmdline.open_pipe_end = -1;
	}
	if (child_i < num_childs -1)
	{
		if (pipe(fds) == -1)
		{
			perror("pipe");
			return (DEADLY_ERR);
		}
		if (redir(fds[1], STDOUT_FILENO) != NO_ERR)
		{
			return (DEADLY_ERR);
		}
		shell->cur_cmdline.open_pipe_end = fds[0];
	}
	return (NO_ERR);
}

int	get_hd_fd(char *hd_str)
{
	int	fds[2];

	if (pipe(fds) == -1)
	{
		perror("pipe");
		return (-1);
	}
	write(fds[1], hd_str, ft_strlen(hd_str));
	if (close(fds[1]) == -1)
	{
		perror("close");
		return (-1);
	}
	return (fds[0]);
}

static t_error	get_io(int fds[2], t_list *redir_lst, char *hd_str)
{
	t_redir	*redir;

	fds[0] = -2;
	fds[1] = -2;
	while (redir_lst)
	{
		redir = get_redir(redir_lst);
		if (redir->type == I_RD_HD || redir->type == I_RD)
		{
			if (input_redirections(fds, redir, hd_str) != NO_ERR)
				return (DEADLY_ERR);
		}
		else if (redir->type == O_RD || redir->type == O_RD_APP)
		{
			if (output_redirections(fds, redir) != NO_ERR)
				return (DEADLY_ERR);
		}
		if (handle_redirection_err(fds, redir) != NO_ERR)
		{
			return (ERR);
		}
		redir_lst = redir_lst->next;
	}
	return (0);
}

static t_error	redir_io(int fds[2])
{
	if (fds[0] >= 0)
	{
		if (dup2(fds[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (DEADLY_ERR);
		}
	}
	if (fds[1] >= 0)
	{
		if (dup2(fds[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return (DEADLY_ERR);
		}
	}
	return (NO_ERR);
}

t_error	set_io_redirs(t_list	*redir_lst, char *hd_str)
{
	t_error	error;
	int		fds[2];

	error = NO_ERR;
	error = get_io(fds, redir_lst, hd_str);
	if (error != NO_ERR)
		return (error);
	error = redir_io(fds);
	if (close_fd(fds[0]) != NO_ERR)
		return (DEADLY_ERR);
	if (close_fd(fds[1]) != NO_ERR)
		return (DEADLY_ERR);
	return (error);
}
