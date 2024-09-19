/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_sec_1_to_6.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/23 12:21:40 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/18 13:06:12 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_error	set_dir(char **directory, char *arg)
{
	char	*home_ptr;

	if (arg)
	{
		*directory = ft_strdup(arg);
		if (!*directory)
			return (perror("malloc"), DEADLY_ERR);
	}
	else
	{
		home_ptr = get_env_val_ptr("HOME");
		if (!home_ptr)
		{
			ft_printf_fd(STDERR_FILENO, "cd: HOME not set\n");
			*directory = NULL;
		}
		else
		{
			*directory = ft_strdup(home_ptr);
			if (!*directory)
				return (perror("malloc"), DEADLY_ERR);
		}
	}
	return (NO_ERR);
}

t_error	init_curpath(char **curpath, char *arg)
{
	char	*finding;
	char	*dir;
	t_error	error;

	*curpath = NULL;
	dir = NULL;
	error = set_dir(&dir, arg);
	if (error || !dir)
		return (error);
	if (dir[0] == '/' || dir[0] == '.' || ft_strncmp(dir, "..", 2) == 0)
	{
		*curpath = dir;
		return (NO_ERR);
	}
	finding = NULL;
	error = try_cdpaths(&finding, dir);
	if (!error && !finding)
		error = try_curdir(&finding, dir);
	free(dir);
	*curpath = finding;
	if (!finding)
		ft_printf_fd(STDERR_FILENO, "cd: %s: No such file or directory\n", arg);
	return (error);
}
