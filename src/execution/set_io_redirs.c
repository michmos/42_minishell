/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_io_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 11:45:20 by mmoser            #+#    #+#             */
/*   Updated: 2024/08/27 11:48:07 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_error	redir_io(int *fd_array, t_list *redir_lst)
{
	int	last_input;
	int	last_output;

	last_input = get_in_idx(redir_lst);
	last_output = get_out_idx(redir_lst);
	if (get_in_idx(redir_lst) > -1)
	{
		if (dup2(fd_array[last_input], STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (SYS_ERR);
		}
	}
	if (last_output > -1)
	{
		if (dup2(fd_array[last_output], STDOUT_FILENO) == -1)
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

static int	open_one_file(t_redir *redir)
{
	int		fd;
	t_tag	type;

	// (void)process; // TODO: see where i use it, or if i need it here
	fd = 0;
	type = redir->type;
	if (type == I_RD)
		fd = open(redir->filename, O_RDONLY);
	else if (type == O_RD)
		fd = open(redir->filename, O_CREAT | O_RDWR | O_TRUNC, 0777);
	else if (type == O_RD_APP)
		fd = open(redir->filename, O_CREAT | O_RDWR | O_APPEND, 0777);

	if (fd == -1)
	{
		perror("open");
	}
	return (fd);
}

static t_error	init_fd_arr(int *fd_array, t_list *redir_lst, char *hd_str)
{
	int	i;
	int	status;

	i = 0;
	while (redir_lst)
	{
		if (get_redir(redir_lst)->type == I_RD_HD)
		{
			fd_array[i] = get_hd_fd(hd_str);
		}
		else
		{
			fd_array[i] = open_one_file(get_redir(redir_lst));
		}

		if (fd_array[i] == -1)
		{
			if (close_fd_array(fd_array, i) != NO_ERR)
			{
				return (SYS_ERR);
			}
			return (SYS_ERR);
		}
		redir_lst = redir_lst->next;
		i++;
	}
	return (0);
}

t_error set_io_redirs(t_list	*redir_lst, char *hd_str)
{
	int	*fd_array;
	size_t	redir_count;
	t_error	error;

	error = NO_ERR;
	redir_count = ft_lstsize(redir_lst);
	if (redir_count == 0)
		return (0);

	// create fd array
	fd_array = (int *)malloc((redir_count * sizeof(int)));
	if (fd_array == NULL)
	{
		perror("malloc");
		return (SYS_ERR);
	}

	// init fd array
	if (init_fd_arr(fd_array, redir_lst, hd_str) != NO_ERR)
	{
		sfree((void **) &fd_array);
		return (SYS_ERR);
	}

	// redirect
	error = redir_io(fd_array, redir_lst);

	// close
	if (close_fd_array(fd_array, redir_count) != NO_ERR)
	{
		sfree((void **) &fd_array);
		return (SYS_ERR);
	}
	sfree((void **) &fd_array);
	return (error);
}

