/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:22:17 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/21 13:42:12 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_error	expand_key(char **str, const char *var_value, size_t pos)
{
	char	*temp;
	size_t	str_len;
	size_t	var_value_len;
	size_t	var_key_len;

	str_len = ft_strlen(*str);
	var_value_len = ft_strlen(var_value);
	var_key_len = 0;

	while((*str) && !ft_is_whitespace((*str)[pos + var_key_len]))
	{
		var_key_len++;
	}
	temp = malloc((str_len - var_key_len + var_value_len) + 1 * sizeof(char));
	if (!temp)
		return (SYS_ERR);
	ft_memcpy(temp, *str, pos);
	ft_memcpy(&temp[pos], var_value, var_value_len);
	ft_memcpy(&temp[pos + var_value_len], &(*str)[pos + var_key_len], str_len - pos - var_key_len);
	free(*str);
	*str = temp;
	return (NO_ERR);
}

t_error	expand_env_var(char **str, t_list *env_lst)
{
	size_t		i;
	const char	*var_value_ptr;

	i = 0;
	while((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			var_value_ptr = get_env_val_ptr(&(*str)[i + 1], env_lst);
			if (var_value_ptr)
			{
				if (expand_key(str, var_value_ptr, i) != NO_ERR)
					; // TODO: protect
				i += ft_strlen(var_value_ptr);
			}
		}
		i++;
	}
	return (NO_ERR);
}
