/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_sec_8_dots1.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/18 16:12:45 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/18 13:32:35 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	is_after_root(char *curpath, size_t i)
{
	return ((i == 1 && curpath[0] == '/') \
	|| (i == 2 && ft_strncmp(curpath, "//", 2) == 0));
}

static bool	prev_comp_is_dir(char *curpath, size_t i)
{
	char	c;
	bool	result;

	c = curpath[i];
	curpath[i] = '\0';
	result = false;
	if (is_dir(curpath))
		result = true;
	curpath[i] = c;
	return (result);
}

static bool	is_slash_before_comp(char *curpath, size_t idx)
{
	return (curpath[idx] == '/' && curpath[idx + 1] != '.' \
	&& curpath[idx + 1] != '/');
}

t_error	rmv_dot_dot_comps(char **curpath, char *og_path)
{
	size_t	i;
	size_t	lst_cmps_pos;

	i = 0;
	while ((*curpath)[i])
	{
		if (is_slash_before_comp(*curpath, i))
			lst_cmps_pos = i + 1;
		if ((*curpath)[i] == '.' && is_affected_dot_dot(*curpath, i))
		{
			if (is_after_root(*curpath, i))
				rmv_dot_dot_slash(*curpath, i);
			else if (prev_comp_is_dir(*curpath, i))
			{
				rmv_dot_dot_slash(*curpath, i);
				rmv_prev_and_slash(*curpath, lst_cmps_pos);
			}
			else
				return (handle_no_dir(*curpath, og_path), NO_ERR);
			i = 0;
		}
		else
			i++;
	}
	return (NO_ERR);
}

void	rmv_dot_comps(char **curpath)
{
	size_t	i;

	i = 0;
	while ((*curpath)[i])
	{
		while ((*curpath)[i] && (*curpath)[i] != '.')
			i++;
		if ((*curpath)[i] == '.' && (i > 0 && (*curpath)[i - 1] == '/'))
		{
			if ((*curpath)[i + 1] == '.')
				i += 2;
			else if ((*curpath)[i + 1] == '/')
			{
				del_char(&(*curpath)[i]);
				del_char(&(*curpath)[i]);
			}
			else if (!(*curpath)[i + 1])
				del_char(&(*curpath)[i]);
			else
				i++;
		}
		else if ((*curpath)[i])
			i++;
	}
}
