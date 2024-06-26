/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:22:17 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/26 17:02:01 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stddef.h>

static size_t	get_key_pos(char *str, size_t pos)
{
	if (ft_strncmp(&str[pos], "${!", 3) == 0)
	{
		return (pos + 3);
	}
	else if (ft_strncmp(&str[pos], "${", 2) == 0)
	{
		return (pos + 2);
	}
	return (pos + 1);
}

static size_t	get_key_len(char *key)
{
	size_t	len;

	len = 0;
	if (*key == '$' || *key == '?')
		return (1);

	while(key[len] && (ft_isalnum(key[len]) || key[len] == '_'))
		len++;
	return (len);
}

static bool	has_syntax_err(char *reference, size_t key_len)
{
	return ((reference[1] == '{' && reference[key_len + 1] != '{')
	     || (reference[1] != '{' && reference[key_len + 1] == '{'));

}

static char	*get_new_str(char *str, char *value_ptr, size_t ref_pos, size_t ref_len)
{
	char	*new_str;
	size_t	new_size;

	new_size = ft_strlen(str) - ref_len + ft_strlen(value_ptr) + 1;
	new_str = ft_calloc(new_size, sizeof(char));
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, str, ref_pos);
	ft_strlcat(new_str, value_ptr, new_size);
	ft_strlcat(new_str, &str[ref_pos + ref_len], new_size);
	return (new_str);
}

static	t_error	replace_var_reference(char **str_ptr, size_t ref_pos, t_list *env_lst)
{
	char	*new_str;
	char	*str;
	char	*value_ptr;
	size_t	key_pos;
	size_t	key_len;
	size_t	ref_len;

	str = *str_ptr;
	key_pos = get_key_pos(str, ref_pos);
	key_len = get_key_len(&str[ref_pos]);
	ref_len = (key_pos - ref_pos) + key_len + (str[key_len] == '}');
	if (has_syntax_err(&str[ref_pos], key_len))
	{
		printf("Syntax Error\n");
		return (SYN_ERR); // TODO: error message to be printed in calling function
	}

	value_ptr = get_env_val_ptr(&str[key_pos], key_len, env_lst);

	new_str = get_new_str(str, value_ptr, ref_pos, ref_len);
	if (!new_str)
		return (SYS_ERR);
	free(str);
	*str_ptr = new_str;
	return (NO_ERR);
}

// TODO: remove linebreak from variable value
//
static	t_error	cut_out_backslash(char **str_ptr, size_t pos)
{
	char	*new_str;
	size_t	len;

	
	len = ft_strlen(*str_ptr) - 1;
	new_str = malloc((len + 1) * sizeof(char));
	if (!new_str)
	{
		return (SYS_ERR);
	}
	ft_memcpy(new_str, *str_ptr, pos);
	ft_strlcat(new_str, *str_ptr, len + 1);
	*str_ptr = new_str;
	return (NO_ERR);
}

static t_error	modify_str(char **str_ptr, size_t pos, t_list *env_lst)
{
	char	*str;
	t_error	error;

	str = *str_ptr;
	error = NO_ERR;
	if (pos > 0 && str[pos - 1] == '\\')
	{
		error = cut_out_backslash(str_ptr, pos - 1);
	}
	else
	{
		error = replace_var_reference(str_ptr, pos, env_lst);
	}
	return (error);
}

t_error	expand_env_var(char **str_ptr, t_list *env_lst)
{
	size_t		i;
	char		*str;
	t_error		error;

	i = 0;
	str = *str_ptr;
	error = NO_ERR;
	while(str[i] && !error)
	{
		if (str[i] == '$' && str[i + 1])
		{
			error = modify_str(str_ptr, i, env_lst);
		}
		i++;
	}
	return (error);
}
