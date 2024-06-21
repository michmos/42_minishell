/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:22:17 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/22 16:16:12 by mmoser           ###   ########.fr       */
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

// static bool	is_key_part(char c)
// {
// 	return (ft_isalnum(c) || c == '_');
//
// }

static char	*get_key_start_ptr(char *str)
{
	bool	brackets_flag;
	char	*key_start;

	if (ft_strncmp(str, "${!", 3) == 0)
	{
		str += 3;
	}
	else if (ft_strncmp(str, "${", 2) == 0)
	{
		str += 2;
	}
	else
		str += 1;
	return (str);
	// while(str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	// {
	// 	i++;
	// }
	// if (str[i] == '}' && brackets_flag)
	// 	i++;
	// // TODO: further checks regarding brackets required?
	// return (i);
}

static t_error	modify_str(char **str_ptr, size_t pos, t_list *env_lst)
{
	char	*str;
	char	*key_start_ptr;
	size_t	cut_out_len;
	size_t	key_len;
	char	*value_ptr;

	str = *str_ptr;
	key_start_ptr = get_key_start_ptr(&str[pos]);
	key_len = 0;
	while (key_start_ptr[key_len] && (ft_isalnum(key_start_ptr[key_len]) || key_start_ptr[key_len] == '_'))
	value_ptr = get_env_val_ptr(key_start_ptr, env_lst);
}

t_error	expand_env_var(char **str_ptr, t_list *env_lst)
{
	size_t		i;
	const char	*var_value_ptr;
	char		*temp;
	char		*str;
	char		*key;
	t_error		error;

	i = 0;
	str = *str_ptr;
	while(str[i])
	{
		if (str[i] == '$' && (i == 0 || str[i - 1] != '\\'))
		{
			error = modify_str(str_ptr, i);
			if (error)
			{
				return (error);
			}
		}
		i++;
	}
	return (NO_ERR);
}
