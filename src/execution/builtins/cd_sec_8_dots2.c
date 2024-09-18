/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_sec_7_to_8.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/18 16:12:45 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/18 13:14:11 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_no_dir(char *curpath, char *og_path)
{
	ft_printf_fd(STDERR_FILENO, "cd: %s: \
No such file or directory\n", og_path);
	ft_bzero(curpath, ft_strlen(curpath));
}

void	rmv_dot_dot_slash(char *curpath, size_t pos)
{
	del_char(&curpath[pos]);
	del_char(&curpath[pos]);
	del_char(&curpath[pos]);
}

void	rmv_prev_and_slash(char *curpath, size_t lst_cmps_pos)
{
	size_t	i;

	i = lst_cmps_pos;
	while (curpath[i] != '/')
		del_char(&curpath[i]);
	del_char(&curpath[i]);
}

bool	is_affected_dot_dot(char *curpath, size_t i)
{
	bool	is_dot_dot;
	bool	is_after_dot_dot;
	bool	is_after_slash;

	is_dot_dot = curpath[i] == '.' && curpath[i + 1] == '.' \
	&& (curpath[i + 2] == '/' || !curpath[i + 2]);
	is_after_dot_dot = i >= 3 && curpath[i - 2] == '.' && curpath[i - 3] == '.';
	is_after_slash = (i > 0) && curpath[i - 1] == '/';
	return (is_dot_dot && is_after_slash && !is_after_dot_dot);
}
