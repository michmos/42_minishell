/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_sec_1_to_6.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/23 12:21:40 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/05 10:45:53 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_error	try_curdir(char **finding, char *dir)
{
	char	*tmp;

	*finding = NULL;
	tmp = ft_strjoin("./", dir);
	if (!tmp)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	if (is_dir(tmp))
	{
		*finding = tmp;
	}
	else
	{
		free(tmp);
	}
	return (NO_ERR);
}

static char	*get_candidate(char *pathname, char *dir)
{
	char	*candidate;
	char	*tmp;

	if (!ft_ends_on(pathname, '/'))
	{
		tmp = ft_strjoin(pathname, "/");
		if (!tmp)
		{
			perror("malloc");
			return (NULL);
		}
		candidate = ft_strjoin(tmp, dir);
		free(tmp);
	}
	else
	{
		candidate = ft_strjoin(pathname, dir);
	}

	if (!candidate)
	{
		perror("malloc");
	}
	return (candidate);
}

static t_error	try_pathname(char **finding, char *pathname, char *dir)
{
	char	*candidate;

	*finding = NULL;
	candidate = get_candidate(pathname, dir);
	if (!candidate)
	{
		return (DEADLY_ERR);
	}
	if (is_dir(candidate))
	{
		*finding = candidate;
	}
	else
	{
		free(candidate);
	}
	return (NO_ERR);
}

// man page section 5
static t_error	try_cdpaths(char **finding, char *dir)
{
	char	**pathnames;
	char	*cd_path_ptr;
	t_error	error;
	size_t	i;

	error = NO_ERR;
	*finding = NULL;

	cd_path_ptr = get_env_val_ptr("CDPATH");
	if (!cd_path_ptr)
		return (NO_ERR);

	pathnames = ft_split(cd_path_ptr, ':');
	if (!pathnames || !*pathnames)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}

	i = 0;
	while (!error && !*finding && pathnames[i])
	{
		error = try_pathname(finding, pathnames[i], dir);
		i++;
	}
	ft_free_2d_array((void **) pathnames);
	return (error);
}

// section 1. and 2.
static t_error set_dir(char **directory, char *arg)
{
	char *home_ptr;

	if (arg)
	{
		*directory = ft_strdup(arg);
		if (!*directory)
		{
			perror("malloc");
			return (DEADLY_ERR);
		}
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
			{
				perror("malloc");
				return (DEADLY_ERR);
			}
		}
	}
	return (NO_ERR);
}

// section 1. - 6.
t_error	init_curpath(char **curpath, char *arg)
{
	char	*finding;
	char	*dir;
	t_error	error;

	*curpath = NULL;
	dir = NULL;
	// section 1. and 2.
	error = set_dir(&dir, arg);
	if (error || !dir)
		return (error);

	// section 3. 4. and 6.
	if (dir[0] == '/' || dir[0] == '.' || ft_strncmp(dir, "..", 2) == 0)
	{
		*curpath = dir;
		return (NO_ERR);
	}

	// section 5.
	finding = NULL;
	error = try_cdpaths(&finding, dir);
	if (!error && !finding)
	{
		error = try_curdir(&finding, dir);
	}
	free(dir);
	*curpath = finding;
	if (!finding)
	{
		ft_printf_fd(STDERR_FILENO, "cd: %s: No such file or directory\n", arg);
	}
	return (error);
}
