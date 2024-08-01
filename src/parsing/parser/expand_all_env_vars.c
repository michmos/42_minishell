/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_all_env_vars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:22:17 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/22 13:05:28 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static	t_error	cut_out_backslash(char **str_ptr, size_t pos)
{
	char	*new_str;
	char	*str;
	size_t	len;

	
	str = *str_ptr;
	len = ft_strlen(str) - 1;
	new_str = ft_calloc((len + 1), sizeof(char));
	if (!new_str)
	{
		return (SYS_ERR);
	}
	ft_memcpy(new_str, str, pos);
	ft_strlcat(new_str, &str[pos + 1], len + 1);
	free(str);
	*str_ptr = new_str;
	return (NO_ERR);
}

static t_error	modify_str(char **str_ptr, size_t *cursor_pos)
{
	char	*str;
	t_error	error;
	size_t	pos;

	str = *str_ptr;
	error = NO_ERR;
	pos = *cursor_pos;
	if (pos > 0 && str[pos - 1] == '\\')
	{
		error = cut_out_backslash(str_ptr, pos - 1);
		*cursor_pos -= 1;
	}
	else if (!str[pos + 1])
	{
		return (NO_ERR);
	}
	else
	{
		error = expand_env_var(str_ptr, cursor_pos);
	}
	return (error);
}

t_error	expand_all_env_vars(char **str_ptr)
{
	size_t	i;
	char	*str;
	t_error	error;

	i = 0;
	str = *str_ptr;
	error = NO_ERR;
	while(str[i] && !error)
	{
		if (str[i] == '$')
		{
			error = modify_str(str_ptr, &i);
			str = *str_ptr;
		}
		i++;
	}
	return (error);
}
