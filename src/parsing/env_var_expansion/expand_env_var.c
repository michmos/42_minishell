/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_env_var.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/01 17:46:04 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/18 11:36:58 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static size_t	get_end_pos(char *str, size_t dollar_pos, bool brackets)
{
	size_t	key_pos;
	size_t	key_len;

	key_pos = get_key_pos(&str[dollar_pos]);
	key_len = get_key_len(&str[dollar_pos + key_pos]);
	return (dollar_pos + key_pos + key_len + brackets);
}

static bool	is_valid_key_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
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

static char	*get_new_str(char *str, size_t startlen, char *insrt, size_t endpos)
{
	char	*new_str;
	char	*end_ptr;
	size_t	new_size;

	end_ptr = &str[endpos];
	new_size = startlen + ft_strlen(insrt) + ft_strlen(end_ptr) + 1;
	new_str = ft_calloc(new_size, sizeof(char));
	if (!new_str)
	{
		perror("malloc");
		return (NULL);
	}
	if (startlen)
		ft_memcpy(new_str, str, startlen);
	if (insrt)
		ft_strlcat(new_str, insrt, new_size);
	if (end_ptr)
		ft_strlcat(new_str, end_ptr, new_size);
	return (new_str);
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
		return (NO_ERR);
	if (brackets && has_error(&str[*dollar_pos]))
		return (ERR);
	if (get_insertion(&insertion, &str[*dollar_pos + brackets]) != NO_ERR)
		return (DEADLY_ERR);
	end_pos = get_end_pos(str, *dollar_pos, brackets);
	new_str = get_new_str(str, *dollar_pos, insertion, end_pos);
	if (!new_str)
		return (free(insertion), DEADLY_ERR);
	*dollar_pos = *dollar_pos + ft_strlen(insertion) - 1;
	free(insertion);
	free(*str_ptr);
	*str_ptr = new_str;
	return (NO_ERR);
}
