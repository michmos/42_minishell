/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_sec_9.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 09:50:28 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/05 10:45:53 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_error	cnvrt_to_rltv_path(char **curpath, char *cwd)
{
	size_t	i;
	char *tmp;

	i = 0;
	while (**curpath && cwd[i] && **curpath == cwd[i])
	{
		del_char(*curpath);
		i++;
	}
	tmp = ft_strjoin("./", *curpath);
	if (!tmp)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	*curpath = tmp;
	return (NO_ERR);
}
