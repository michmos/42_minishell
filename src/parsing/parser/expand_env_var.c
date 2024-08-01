/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 17:46:04 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/22 13:40:46 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stddef.h>

static size_t	get_key_pos(char *ref_start)
{
	if (ft_strncmp(ref_start, "${", 2) == 0)
	{
		return (2);
	}
	return (1);
}

static size_t	get_key_len(char *key)
{
	size_t	len;

	len = 0;
	if (*key == '$' || *key == '?')
		return (1);

	while(key[len] && (ft_isalnum(key[len]) || key[len] == '_'))
	{
		len++;
	}
	return (len);
}

static size_t	get_end_pos(char *str, size_t ref_pos, bool brackets)
{
	size_t	key_pos;
	size_t	key_len;

	key_pos = get_key_pos(str);
	key_len = get_key_len(&str[key_pos]);
	return (ref_pos + key_pos + key_len + brackets);
}

static bool	is_valid_key_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static t_error	has_syn_error(char *str)
{
	size_t	i;
	bool	bad_substition;

	i = 0;
	bad_substition = false;
	while (str[i] && str[i] != '}')
	{
		if (!is_valid_key_char(str[i]))
			bad_substition = true;
		i++;
	}
	if (str[i] != '}')
	{
		printf("Syntax error: missing closing bracket");
		return (SYN_ERR);
	}
	else if (bad_substition)
	{
		printf ("Bad substition\n");
		return (SYN_ERR);
	}
	return (NO_ERR);
}

static char	*get_new_str(char *start, size_t start_len, char *insertion, size_t	end_pos)
{
	char	*new_str;
	char	*end_ptr;
	size_t	new_size;

	end_ptr = &start[end_pos];
	new_size = start_len + ft_strlen(insertion) + ft_strlen(end_ptr) + 1;
	new_str = ft_calloc(new_size, sizeof(char));
	if (!new_str)
		return (NULL);
	if (start_len)
		ft_memcpy(new_str, start, start_len);
	if (insertion)
		ft_strlcat(new_str, insertion, new_size);
	if (end_ptr)
		ft_strlcat(new_str, end_ptr, new_size);
	return (new_str);
}

// TODO: cleanup function
static t_error	get_var_value(char	**insertion, char *ref_start)
{
	char	*value;
	char	*ptr;
	char	*key;
	size_t	key_pos;
	size_t	key_len;

	key_pos = get_key_pos(ref_start);
	key_len = get_key_len(&ref_start[key_pos]);
	key = ft_substr(ref_start, key_pos, key_len);
	if (!key)
		return (SYS_ERR);
	if (*key == '$')
	{
		value = ft_itoa((int) getpid());
	}
	else if (*key == '?')
	{
		value = ft_itoa((int) get_exit_code());
	}
	else
	{
		ptr = get_env_val_ptr(key);
		if (!ptr)
		{
			free(key);
			*insertion = NULL;
			return (NO_ERR);
		}
		value = ft_strdup(ptr);
	}

	free(key);
	if (!value)
		return (SYS_ERR);
	*insertion = value;
	return (NO_ERR);
}

t_error	expand_env_var(char **str_ptr, size_t *cur_pos)
{
	char	*str;
	char	*insertion;
	char	*new_str;
	size_t	end_pos;
	bool	brackets;

	str = *str_ptr;
	brackets = (str[*cur_pos + 1] == '{');
	if (!brackets && !is_valid_key_char(str[*cur_pos + 1]))
	{
		return (NO_ERR);
	}
	if (brackets && has_syn_error(&str[*cur_pos + 2]))
	{
		return (SYN_ERR);
	}
	if (get_var_value(&insertion, &str[*cur_pos + brackets]) != NO_ERR)
	{
		return (SYS_ERR);
	}
	end_pos = get_end_pos(str, *cur_pos, brackets);
	new_str = get_new_str(str, *cur_pos, insertion, end_pos);
	if (!new_str)
	{
		free(insertion);
		return (SYS_ERR);
	}
	*cur_pos = *cur_pos + ft_strlen(insertion) - 1;
	free(insertion);
	free(*str_ptr);
	*str_ptr = new_str;
	return (NO_ERR);
}
