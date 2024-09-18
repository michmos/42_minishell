/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   add_char.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/02 10:16:59 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/18 11:27:55 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	vec_is_full(t_vec *vector)
{
	size_t	len;

	len = ft_strlen(vector->buf);
	return (len + 1 >= vector->size);
}

static t_error	extend_vec(t_vec *vector)
{
	char	*temp;

	temp = ft_realloc(vector->buf, vector->size * 2, vector->size);
	if (!temp)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	ft_bzero(&temp[vector->size], vector->size);
	vector->buf = temp;
	vector->size *= 2;
	return (NO_ERR);
}

t_error	add_char(char c, t_vec *vector)
{
	if (vec_is_full(vector))
	{
		if (extend_vec(vector) == DEADLY_ERR)
			return (DEADLY_ERR);
	}
	vector->buf[ft_strlen(vector->buf)] = c;
	return (NO_ERR);
}
