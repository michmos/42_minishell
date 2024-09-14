/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fds.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/29 10:02:34 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/06 12:05:14 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

t_error	set_io_pipes(size_t child_i, size_t num_childs)
{
	int		fds[2];
	t_shell	*shell;

	shell = get_shell_struct();
	if (child_i > 0) // redirect stdin to pipe if not first child
	{
		if (redir(shell->open_fd, STDIN_FILENO) != NO_ERR)
			return (DEADLY_ERR);
		shell->open_fd = -1;
	}
	if (child_i < num_childs -1) // redirect stdout to pipe if not last child
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
		// save unused read end
		shell->open_fd = fds[0];
	}
	return (NO_ERR);
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

static int	get_hd_fd(char *hd_str)
{
	int	fds[2];
	// redirect stdin to pipe if not first child
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
	int		i;
	t_redir	*redir;

	i = 0;
	fds[0] = -2;
	fds[1] = -2;

	while (redir_lst)
	{
		redir = get_redir(redir_lst);
		if (redir->type == I_RD_HD)
		{
			if (close_fd(fds[0]) != NO_ERR)
			{
				return (DEADLY_ERR);
			}
			fds[0] = get_hd_fd(hd_str);
		}
		else if (redir->type == I_RD)
		{
			if (close_fd(fds[0]) != NO_ERR)
			{
				return (DEADLY_ERR);
			}
			fds[0] = open(redir->filename, O_RDONLY);
		}
		else if (redir->type == O_RD)
		{
			if (close_fd(fds[1]) != NO_ERR)
			{
				return (DEADLY_ERR);
			}
			fds[1] = open(redir->filename, O_CREAT | O_RDWR | O_TRUNC, 0777);
		}
		else if (redir->type == O_RD_APP)
		{
			if (close_fd(fds[1]) != NO_ERR)
			{
				return (DEADLY_ERR);
			}
			fds[1] = open(redir->filename, O_CREAT | O_RDWR | O_APPEND, 0777);
		}




		if (fds[0] == -1 || fds[1] == -1)
		{
			if (close_fd(fds[0]) != NO_ERR)
			{
				return (DEADLY_ERR);
			}
			if (close_fd(fds[1]) != NO_ERR)
			{
				return (DEADLY_ERR);
			}
			ft_printf_fd(STDERR_FILENO, "%s: %s: %s\n", SHELLNAME, redir->filename, strerror(errno));
			return (ERR);
		}
		redir_lst = redir_lst->next;
		i++;
	}
	return (0);
}

t_error	set_io_redirs(t_list	*redir_lst, char *hd_str)
{
	t_error	error;
	int		fds[2];

	error = NO_ERR;

	// init fd array
	error = get_io(fds, redir_lst, hd_str);
	if (error != NO_ERR)
		return (error);

	// redirect
	error = redir_io(fds);
	// close
	if (close_fd(fds[0]) != NO_ERR)
		return (DEADLY_ERR);
	if (close_fd(fds[1]) != NO_ERR)
		return (DEADLY_ERR);
	return (error);
}
