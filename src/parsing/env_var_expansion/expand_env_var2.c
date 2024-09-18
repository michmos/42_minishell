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

size_t	get_key_pos(char *ref_start)
{
	if (ft_strncmp(ref_start, "${", 2) == 0)
	{
		return (2);
	}
	return (1);
}

size_t	get_key_len(char *key)
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
		*value = ft_itoa((int) getpid());
	else if (*key == '?')
		*value = ft_itoa((int) get_exit_code());
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

t_error	get_insertion(char	**insertion, char *ref_start)
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
