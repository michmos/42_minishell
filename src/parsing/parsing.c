/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:05:11 by mmoser            #+#    #+#             */
/*   Updated: 2024/05/02 11:57:24 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parsing(char *str)
{
	t_parse_str	cmd_line;

	cmd_line.buf = str;
	cmd_line.buf_len = ft_strlen(str);
	cmd_line.cursor_pos = 0;
	lexer(&cmd_line);
}
