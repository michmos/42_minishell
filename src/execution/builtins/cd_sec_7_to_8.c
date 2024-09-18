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

static t_error	cnvrt_to_canonical(char **curpath)
{
	char	*result;
	char	*og_path;

	og_path = ft_strdup(*curpath);
	if (!og_path)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	smplfy_lead_slashes(*curpath);
	smplfy_inbtw_slashes(*curpath);
	rmv_dot_comps(curpath);
	rmv_dot_dot_comps(curpath, og_path);
	free(og_path);
	smplfy_trail_slashes(*curpath);
	if (!**curpath)
		return (NO_ERR);
	result = ft_strdup(*curpath);
	sfree((void **) curpath);
	if (!result)
		return (perror("malloc"), DEADLY_ERR);
	*curpath = result;
	return (NO_ERR);
}

static t_error	concatenate_to_pwd(char **curpath)
{
	t_shell	*shell;
	char	*tmp;
	char	*pwd;

	shell = get_shell_struct();
	pwd = ft_strdup(shell->cwd);
	if (!pwd)
		return (perror("malloc"), DEADLY_ERR);
	if (!ft_ends_on(pwd, '/'))
	{
		if (add_slash(&pwd) != NO_ERR)
		{
			free(pwd);
			return (DEADLY_ERR);
		}
	}
	tmp = ft_strjoin(pwd, *curpath);
	free(pwd);
	if (!tmp)
		return (perror("malloc"), DEADLY_ERR);
	free(*curpath);
	*curpath = tmp;
	return (NO_ERR);
}

t_error	modify_curpath(char **curpath)
{
	t_error	error;

	if ((*curpath)[0] != '/')
	{
		if (concatenate_to_pwd(curpath) != NO_ERR)
		{
			return (DEADLY_ERR);
		}
	}
	error = cnvrt_to_canonical(curpath);
	if (error || !**curpath)
	{
		return (error);
	}
	return (NO_ERR);
}
