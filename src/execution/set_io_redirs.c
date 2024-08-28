/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_io_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 11:45:20 by mmoser            #+#    #+#             */
/*   Updated: 2024/08/28 10:04:56 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

static t_error	redir_io(int fds[2])
{
	if (fds[0] >= 0)
	{
		if (dup2(fds[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (SYS_ERR);
		}
	}
	if (fds[1] >= 0)
	{
		if (dup2(fds[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return (SYS_ERR);
		}
	}
	return (NO_ERR);
}

static int	get_hd_fd(char *hd_str)
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
	int	i;
	int	status;
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
				return (SYS_ERR);
			}
			fds[0] = get_hd_fd(hd_str);
		}
		else if (redir->type == I_RD)
		{
			if (close_fd(fds[0]) != NO_ERR)
			{
				return (SYS_ERR);
			}
			fds[0] = open(redir->filename, O_RDONLY);
		}
		else if (redir->type == O_RD)
		{
			if (close_fd(fds[1]) != NO_ERR)
			{
				return (SYS_ERR);
			}
			fds[1] = open(redir->filename, O_CREAT | O_RDWR | O_TRUNC, 0777);
		}
		else if (redir->type == O_RD_APP)
		{
			if (close_fd(fds[1]) != NO_ERR)
			{
				return (SYS_ERR);
			}
			fds[1] = open(redir->filename, O_CREAT | O_RDWR | O_APPEND, 0777);
		}




		if (fds[0] == -1 || fds[1] == -1)
		{
			if (close_fd(fds[0]) != NO_ERR)
			{
				return (SYS_ERR);
			}
			if (close_fd(fds[1]) != NO_ERR)
			{
				return (SYS_ERR);
			}
			ft_printf_fd(STDERR_FILENO, "%s: %s: %s\n", SHELLNAME, redir->filename, strerror(errno));
			return (SYS_ERR);
		}
		redir_lst = redir_lst->next;
		i++;
	}
	return (0);
}

t_error set_io_redirs(t_list	*redir_lst, char *hd_str)
{
	t_error	error;
	int		fds[2];

	error = NO_ERR;

	// init fd array
	if (get_io(fds, redir_lst, hd_str) != NO_ERR)
	{
		return (SYS_ERR);
	}

	// redirect
	error = redir_io(fds);

	// close
	if (close_fd(fds[0]) != NO_ERR)
	{
		return (SYS_ERR);
	}
	if (close_fd(fds[1]) != NO_ERR)
	{
		return (SYS_ERR);
	}
	return (error);
}

