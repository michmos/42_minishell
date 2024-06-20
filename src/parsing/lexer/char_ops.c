/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 10:23:56 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/17 16:23:29 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	cur_char(t_parse_str *str)
{
	return (str->buf[str->cursor_pos]);
}

void	advance_char(t_parse_str *str, size_t n)
{
	if (str->cursor_pos + n >= str->buf_len)
	{
		str->cursor_pos = str->buf_len;
		return ;
	}
	str->cursor_pos += n;
}

char	peek_char(t_parse_str *str)
{
	if (cur_char(str)== '\0')
		return (-1);
	return (str->buf[str->cursor_pos + 1]);
}
