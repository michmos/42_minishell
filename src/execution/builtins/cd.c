/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/05 12:04:52 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/18 13:05:45 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_error	set_curpath(char **curpath, char *arg, char *old_wd)
{
	t_error	error;

	*curpath = NULL;
	if (arg && ft_strncmp(arg, "-", 2) == 0)
	{
		*curpath = ft_strdup(old_wd);
		if (!*curpath)
			return (perror("malloc"), DEADLY_ERR);
		return (NO_ERR);
	}
	error = init_curpath(curpath, arg);
	if (error || !*curpath)
	{
		sfree((void **) curpath);
		return (error);
	}
	error = modify_curpath(curpath);
	if (error || (*curpath && !**curpath))
		sfree((void **) curpath);
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
	char	*curpath;
	t_shell	*shell;

	if (argv[1] && argv[2])
		return (ft_printf_fd(STDERR_FILENO, "cd: too many arguments\n"), ERR);
	curpath = NULL;
	shell = get_shell_struct();
	error = set_curpath(&curpath, argv[1], shell->old_wd);
	if (error || !curpath)
		return (ERR);
	if (set_pwd(curpath) != NO_ERR)
		return (free(curpath), DEADLY_ERR);
	if (ft_strnstr(curpath, shell->old_wd, ft_strlen(shell->old_wd)) != NULL)
		error = cnvrt_to_rltv_path(&curpath, shell->old_wd);
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
