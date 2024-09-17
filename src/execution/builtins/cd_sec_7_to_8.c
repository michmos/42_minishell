/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_sec_7_to_8.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/18 16:12:45 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/05 10:45:53 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	smplfy_lead_slashes(char *str)
{
	size_t	count;

	if (!str || !*str)
		return ;

	count = 0;
	while (str[count] && str[count] == '/')
	{
		count++;
	}
	if (count > 2)
	{
		ft_memmove(&str[1], &str[count], ft_strlen(&str[count]) + 1);
	}
}

static void	smplfy_trail_slashes(char *str)
{
	size_t	len;
	size_t	i;

	if (!str || !*str)
		return ;

	len = ft_strlen(str);
	i = len - 1;
	while (i > 0 && str[i] == '/')
	{
		i--;
	}
	if (i > 0)
	{
		ft_memset(&str[i + 1], '\0', 1);
	}
}

static void	smplfy_inbtw_slashes(char *str)
{
	size_t	i;

	i = 0;
	if (!str || !*str)
		return ;

	while (str[i] && str[i] == '/')
	{
		i++;
	}

	while (str[i])
	{
		while (str[i] && str[i] != '/')
		{
			i++;
		}
		if (str[i] == '/')
		{
			i++;
		}
		while (str[i] && str[i] == '/')
		{
			del_char(&str[i]);
		}
	}
}

static void	rmv_dot_dot_slash(char *curpath, size_t pos)
{
	// del dot dots
	del_char(&curpath[pos]);
	del_char(&curpath[pos]);
	// del slash
	del_char(&curpath[pos]);
}

static void	rmv_prev_and_slash(char *curpath, size_t lst_cmps_pos)
{
	size_t i;

	i = lst_cmps_pos;
	// del prev component
	while(curpath[i] != '/')
		del_char(&curpath[i]);
	// del slash
	del_char(&curpath[i]);
}

static bool	is_affected_dot_dot(char *curpath, size_t i)
{
	bool	is_dot_dot;
	bool	is_after_dot_dot;
	bool	is_after_slash;

	is_dot_dot = curpath[i] == '.' && curpath[i + 1] == '.' && (curpath[i + 2] == '/' || !curpath[i + 2]);
	is_after_dot_dot = i >= 3 && curpath[i - 2] == '.' && curpath[i - 3] == '.';
	is_after_slash = (i > 0) && curpath[i - 1] == '/';
	return (is_dot_dot && is_after_slash && !is_after_dot_dot);
}

static bool	is_after_root(char *curpath, size_t i)
{
	return ((i == 1 && curpath[0] == '/') || (i == 2 && ft_strncmp(curpath, "//", 2) == 0));

}

static bool	prev_comp_is_dir(char *curpath, size_t i)
{
	char c;
	bool result;

	c = curpath[i];
	curpath[i] = '\0';
	result = false;
	if (is_dir(curpath))
		result = true;
	curpath[i] = c;
	return (result);
}

static t_error	rmv_dot_dot_comps(char **curpath, char *og_path)
{
	size_t	i;
	size_t  lst_cmps_pos;

	i = 0;
	while ((*curpath)[i])
	{
		if ((*curpath)[i] == '/' && (*curpath)[i + 1] != '.' && (*curpath)[i + 1] != '/')
			lst_cmps_pos = i + 1;
		if ((*curpath)[i] == '.' && is_affected_dot_dot(*curpath, i))
		{
			if (is_after_root(*curpath, i))
			{
				rmv_dot_dot_slash(*curpath, i);
			}
			else if (prev_comp_is_dir(*curpath, i))
			{
				rmv_dot_dot_slash(*curpath, i);
				rmv_prev_and_slash(*curpath, lst_cmps_pos);
			}
			else
			{
				ft_printf_fd(STDERR_FILENO, "cd: %s: No such file or directory\n", og_path);
				ft_bzero(*curpath, ft_strlen(*curpath));
				return (NO_ERR);
			}
			i = 0;
		}
		else
			i++;
	}
	return (NO_ERR);
}



static void rmv_dot_comps(char **curpath)
{
	size_t	i;

	i = 0;
	while ((*curpath)[i])
	{
		while ((*curpath)[i] && (*curpath)[i] != '.')
		{
			i++;
		}
		if ((*curpath)[i] == '.' && (i > 0 && (*curpath)[i - 1] == '/'))
		{
			if ((*curpath)[i + 1] == '.')
			{
				i += 2;
			}
			else if ((*curpath)[i + 1] == '/')
			{
				del_char(&(*curpath)[i]);
				del_char(&(*curpath)[i]);
			}
			else if (!(*curpath)[i + 1])
			{
				del_char(&(*curpath)[i]);
			}
			else
				i++;
		}
		else if ((*curpath)[i])
			i++;
	}
}

static t_error	cnvrt_to_canonical(char **curpath)
{
	char 	*result;
	char 	*og_path;

	og_path = ft_strdup(*curpath);
	if (!og_path)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}

	// section 8 c. part 1
	smplfy_lead_slashes(*curpath);
	smplfy_inbtw_slashes(*curpath);
	// section 8 a.
	rmv_dot_comps(curpath);
	// section 8 b.
	rmv_dot_dot_comps(curpath, og_path);
	free(og_path);
	// section 8 c. part 2
	smplfy_trail_slashes(*curpath);

	// no further steps if empty string left
	if (!**curpath)
	{
		return (NO_ERR);
	}

	result = ft_strdup(*curpath);
	sfree((void **) curpath);
	if (!result)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	*curpath = result;
	return (NO_ERR);
}

// section 7.
static t_error	concatenate_to_pwd(char **curpath)
{
	t_shell	*shell;
	char	*tmp;
	char	*pwd;

	shell = get_shell_struct();
	pwd = ft_strdup(shell->cwd);
	if (!pwd)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	if (!ft_ends_on(pwd, '/'))
	{
		if (add_slash(&pwd)!= NO_ERR)
		{
			free(pwd);
			return (DEADLY_ERR);
		}
	}
	tmp = ft_strjoin(pwd, *curpath);
	free(pwd);
	if (!tmp)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	free(*curpath);
	*curpath = tmp;
	return (NO_ERR);
}


t_error	modify_curpath(char **curpath)
{
	t_error	error;

	// section 7.
	if ((*curpath)[0] != '/')
	{
		if (concatenate_to_pwd(curpath) != NO_ERR)
		{
			return (DEADLY_ERR);
		}
	}

	// section 8.
	error = cnvrt_to_canonical(curpath);
	if (error || !**curpath)
	{
		return (error);
	}
	return (NO_ERR);
}
