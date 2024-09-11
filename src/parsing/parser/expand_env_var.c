/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_env_var.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/01 17:46:04 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/11 10:52:08 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stddef.h>
#include <unistd.h>

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

	while (key[len] && (ft_isalnum(key[len]) || key[len] == '_'))
	{
		len++;
	}
	return (len);
}

static size_t	get_end_pos(char *str, size_t dollar_pos, bool brackets)
{
	size_t	key_pos;
	size_t	key_len;

	key_pos = get_key_pos(str);
	key_len = get_key_len(&str[dollar_pos + key_pos]);
	return (dollar_pos + key_pos + key_len + brackets);
}

static bool	is_valid_key_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static t_error	has_error(char *str)
{
	size_t	i;
	bool	bad_substition;

	i = 2;
	bad_substition = false;
	while (str[i] && str[i] != '}')
	{
		if (!is_valid_key_char(str[i]))
			bad_substition = true;
		i++;
	}
	if (str[i] != '}')
	{
		ft_printf_fd(STDERR_FILENO, "syntax error: closing } is missing\n");
		return (ERR);
	}
	else if (bad_substition)
	{
		ft_printf_fd(STDERR_FILENO, "syntax error: %s: bad substition\n", str);
		return (ERR);
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
	{
		perror("malloc");
		return (NULL);
	}
	if (start_len)
		ft_memcpy(new_str, start, start_len);
	if (insertion)
		ft_strlcat(new_str, insertion, new_size);
	if (end_ptr)
		ft_strlcat(new_str, end_ptr, new_size);
	return (new_str);
}

static t_error	get_var_key(char **key, char *ref_start)
{
	size_t	key_pos;
	size_t	key_len;

	key_pos = get_key_pos(ref_start);
	key_len = get_key_len(&ref_start[key_pos]);
	*key = ft_substr(ref_start, key_pos, key_len);
	if (!*key)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	return (NO_ERR);
}

static t_error	get_var_value(char **value, char *key)
{
	char	*ptr;

	if (*key == '$')
	{
		*value = ft_itoa((int) getpid());
	}
	else if (*key == '?')
	{
		*value = ft_itoa((int) get_exit_code());
	}
	else
	{
		ptr = get_env_val_ptr(key);
		if (!ptr)
		{
			*value = NULL;
			return (NO_ERR);
		}
		*value = ft_strdup(ptr);
		if (!value)
		{
			perror("malloc");
			return (DEADLY_ERR);
		}
	}
	return (NO_ERR);
}

static t_error	get_insertion(char	**insertion, char *ref_start)
{
	char	*value;
	char	*key;

	if (get_var_key(&key, ref_start) != NO_ERR)
	{
		return (DEADLY_ERR);
	}
	if (get_var_value(&value, key) != NO_ERR)
	{
		free(key);
		return (DEADLY_ERR);
	}
	free(key);
	*insertion = value;
	return (NO_ERR);
}

t_error	expand_env_var(char **str_ptr, size_t *dollar_pos)
{
	char	*str;
	char	*insertion;
	char	*new_str;
	size_t	end_pos;
	bool	brackets;

	str = *str_ptr;
	brackets = (str[*dollar_pos + 1] == '{');
	if (!brackets && !is_valid_key_char(str[*dollar_pos + 1]))
	{
		return (NO_ERR);
	}
	if (brackets && has_error(&str[*dollar_pos]))
	{
		return (ERR);
	}
	if (get_insertion(&insertion, &str[*dollar_pos + brackets]) != NO_ERR)
	{
		return (DEADLY_ERR);
	}
	end_pos = get_end_pos(str, *dollar_pos, brackets);
	new_str = get_new_str(str, *dollar_pos, insertion, end_pos);
	if (!new_str)
	{
		free(insertion);
		return (DEADLY_ERR);
	}
	*dollar_pos = *dollar_pos + ft_strlen(insertion) - 1;
	free(insertion);
	free(*str_ptr);
	*str_ptr = new_str;
	return (NO_ERR);
}
