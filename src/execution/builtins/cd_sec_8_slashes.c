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

void	smplfy_lead_slashes(char *str)
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

void	smplfy_trail_slashes(char *str)
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

void	smplfy_inbtw_slashes(char *str)
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
