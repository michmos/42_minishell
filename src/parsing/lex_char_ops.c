/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_char_ops.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 10:23:56 by mmoser            #+#    #+#             */
/*   Updated: 2024/05/06 12:10:00 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	cur_char(t_parse_str *str)
{
	return (str->buf[str->cursor_pos]);
}

char	next_char(t_parse_str *cmd_line)
{
	cmd_line->cursor_pos++;
	return (cmd_line->buf[cmd_line->cursor_pos]);
}

char	peek_char(t_parse_str *cmd_line)
{
	if (cur_char(cmd_line)== '\0')
		return (-1);
	return (cmd_line->buf[cmd_line->cursor_pos + 1]);
}
