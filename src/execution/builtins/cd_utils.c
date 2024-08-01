/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:19:25 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/31 16:21:38 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_error	add_slash(char **str)
{
	char	*result;

	if (!str || !*str)
	{
		return (INP_ERR);
	}
	result = ft_strjoin(*str, "/");
	if (!result)
		return (SYS_ERR);
	free(*str);
	*str = result;
	return (NO_ERR);
}

bool	is_dir(char *pathname)
{
	struct stat statbuff;

	if (stat(pathname, &statbuff) != 0)
		return (false);
	return (S_ISDIR(statbuff.st_mode));
}

void del_char(char *ptr)
{
	ft_memmove(ptr, ptr + 1, ft_strlen(ptr + 1) + 1);
}

