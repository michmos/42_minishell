/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var_ref.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 17:46:04 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/01 18:00:16 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
		len++;
	return (len);
}

static bool	has_syntax_err(char a, char b)
{
	return ((a == '{' && b != '}')
	     || (a != '{' && b == '}'));
}

static char	*get_new_str(char *start, size_t start_len, char *insertion, char *end)
{
	char	*new_str;
	size_t	new_size;

	new_size = start_len + ft_strlen(insertion) + ft_strlen(end) + 1;
	new_str = ft_calloc(new_size, sizeof(char));
	if (!new_str)
		return (NULL);
	if (start_len)
		ft_memcpy(new_str, start, start_len);
	if (insertion)
		ft_strlcat(new_str, insertion, new_size);
	if (end)
		ft_strlcat(new_str, end, new_size);
	return (new_str);
}

static t_error	get_var_value(char	**insertion, char *ref_start, t_list *env_lst)
{
	char	*value;
	char	*ptr;
	size_t	key_pos;
	size_t	key_len;

	ptr = NULL;
	key_pos = get_key_pos(ref_start);
	key_len = get_key_len(&ref_start[key_pos]);
	if (ref_start[key_pos] == '$')
	{
		value = ft_itoa((int) getpid());
	}
	else if (ref_start[key_pos] == '?')
	{
		value = ft_itoa((int) get_exit_code());
	}
	else
	{
		ptr = get_env_val_ptr(&ref_start[key_pos], key_len, env_lst);
		if (ptr)
		{
			value = ft_strdup(ptr);
		}
		else
		{
			value = ptr;
		}
	}
	if (!value && ptr)
		return (SYS_ERR);
	*insertion = value;
	return (NO_ERR);
}

static	size_t	get_ref_len(char *ref_start)
{
	size_t	key_pos;
	size_t	key_len;

	key_pos = get_key_pos(ref_start);
	key_len = get_key_len(&ref_start[key_pos]);
	return (key_pos + key_len + (ref_start[key_pos + key_len] == '}'));
}

t_error	expand_env_var(char **str_ptr, size_t *cursor_pos, t_list *env_lst)
{
	char	*start;
	char	*insertion;
	char	*end;
	char	*new_str;
	size_t	ref_len;

	start = *str_ptr;
	ref_len = get_ref_len(&start[*cursor_pos]);
	if (has_syntax_err(start[*cursor_pos + 1], start[*cursor_pos + ref_len - 1]))
	{
		printf("Syntax error\n");
		return (SYN_ERR);
	}
	if (get_var_value(&insertion, &start[*cursor_pos], env_lst) != NO_ERR)
	{
		return (SYS_ERR);
	}
	end = &start[*cursor_pos + ref_len];
	new_str = get_new_str(start, *cursor_pos, insertion, end);
	*cursor_pos = *cursor_pos + ft_strlen(insertion) - 1;
	free(insertion);
	if (!new_str)
	{
		return (SYS_ERR);
	}
	free(*str_ptr);
	*str_ptr = new_str;
	return (NO_ERR);
}
