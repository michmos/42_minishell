/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/05 12:04:52 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/05 10:45:53 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	is_dir(char *pathname)
{
	struct stat statbuff;

	if (stat(pathname, &statbuff) != 0)
		return (false);
	return (S_ISDIR(statbuff.st_mode));
}

// section 1. - 8.
static t_error	set_curpath(char **curpath, char *arg, char *old_wd)
{
	t_error	error;

	*curpath = NULL;
	// cd - functionality
	if (arg && ft_strncmp(arg, "-", 2) == 0)
	{
		*curpath = ft_strdup(old_wd);
		if (!*curpath)
		{
			perror("malloc");
			return (DEADLY_ERR);
		}
		return (NO_ERR);
	}

	// section 1. - 6.
	error = init_curpath(curpath, arg);
	if (error || !*curpath)
	{
		sfree((void **) curpath);
		return (error);
	}

	// section 7. and 8.
	error = modify_curpath(curpath);
	if (error || (*curpath && !**curpath))
	{
		sfree((void **) curpath);
	}
	else if (!is_dir((*curpath)))
	{
		ft_printf_fd(STDERR_FILENO, "cd: %s: No such file or directory\n", arg);
		sfree((void **) curpath);
	}
	return (error);
}

t_error	cd(char *argv[])
{
	t_error	error;
	char	*curpath; // allocated
	t_shell	*shell;

	if (argv[1] && argv[2])
	{
		ft_printf_fd(STDERR_FILENO, "cd: too many arguments\n");
		return (ERR);
	}

	curpath = NULL;
	shell = get_shell_struct();
	// section 1. - 8.
	error = set_curpath(&curpath, argv[1], shell->old_wd);
	if (error || !curpath)
	{
		return (ERR);
	}
	// section 10.
	if (set_pwd(curpath) != NO_ERR)
	{
		free(curpath);
		return (DEADLY_ERR);
	}
	// section 9.
	if (ft_strnstr(curpath, shell->old_wd, ft_strlen(shell->old_wd)) != NULL)
	{
		error = cnvrt_to_rltv_path(&curpath, shell->old_wd);
	}
	// section 10.
	if (!error)
	{
		if (chdir(curpath) == -1)
		{
			perror("cd");
			error = ERR;
		}
	}
	free(curpath);
	return (error);
}
