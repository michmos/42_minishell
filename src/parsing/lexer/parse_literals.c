/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_literals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 21:11:23 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/17 16:32:04 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*get_substr_until(t_parse_str *cmd_line, const char *seperators)
{
	t_vec	vec;
	char	*substr;

	vec.size = VEC_START_SZ;
	vec.buf = ft_calloc(sizeof(char), vec.size);
	if (!vec.buf)
		return(NULL);

	while(cur_char(cmd_line) && !ft_strchr(seperators, cur_char(cmd_line)))
	{
		if (add_char(cur_char(cmd_line), &vec) != NO_ERR)
		{
			free(vec.buf);
			return (NULL);
		}
		advance_char(cmd_line, 1);
	}
	substr = ft_realloc(vec.buf, ft_strlen(vec.buf) + 1, vec.size);
	if (!substr)
	{
		free(vec.buf);
		return (NULL);
	}
	return (substr);
}

t_error	parse_str_dq(char **lexeme, t_parse_str *cmd_line)
{
	t_error	error;

	advance_char(cmd_line, 1);
	*lexeme = get_substr_until(cmd_line, "\"");
	if (!*lexeme)
		return (SYS_ERR);

	if (cur_char(cmd_line) != '\"')
	{
		printf("syntaxerror\n"); // TODO: display syntax error
		return (SYN_ERR);
	}
	advance_char(cmd_line, 2);
	return (NO_ERR);
}

t_error	parse_str_sq(char **lexeme, t_parse_str *cmd_line)
{
	t_error	error;

	advance_char(cmd_line, 1);
	*lexeme = get_substr_until(cmd_line, "\'");
	if (!*lexeme)
		return (SYS_ERR);

	if (cur_char(cmd_line) != '\"')
	{
		printf("syntaxerror\n"); // TODO: display syntax error
		return (SYN_ERR);
	}
	advance_char(cmd_line, 2);
	return (NO_ERR);
}

t_error	parse_word(char **lexeme, t_parse_str *cmd_line)
{
	*lexeme = get_substr_until(cmd_line, " <>|\"\'");
	if (!*lexeme)
	{
		return (SYS_ERR);
	}
	return (NO_ERR);
}
