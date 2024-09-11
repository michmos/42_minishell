/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_literals.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/15 21:11:23 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/11 10:55:21 by pminialg      ########   odam.nl         */
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
		return (perror("malloc"), NULL);
	while (cur_char(cmd_line) && !ft_strchr(seperators, cur_char(cmd_line)))
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
		perror("malloc");
		free(vec.buf);
		return (NULL);
	}
	return (substr);
}

t_error	parse_str_dq(char **lexeme, t_parse_str *cmd_line)
{
	advance_char(cmd_line, 1);
	*lexeme = get_substr_until(cmd_line, "\"");
	if (!*lexeme)
		return (DEADLY_ERR);
	if (cur_char(cmd_line) != '\"')
	{
		sfree((void **) lexeme);
		ft_printf_fd(STDERR_FILENO, \
		"syntax error: missing closing double quotation mark\n");
		return (ERR);
	}
	advance_char(cmd_line, 2);
	return (NO_ERR);
}

t_error	parse_str_sq(char **lexeme, t_parse_str *cmd_line)
{
	advance_char(cmd_line, 1);
	*lexeme = get_substr_until(cmd_line, "\'");
	if (!*lexeme)
		return (DEADLY_ERR);

	if (cur_char(cmd_line) != '\'')
	{
		sfree((void **) lexeme);
		ft_printf_fd(STDERR_FILENO, \
		"syntax error: missing closing single quotation mark\n");
		return (ERR);
	}
	advance_char(cmd_line, 2);
	return (NO_ERR);
}

t_error	parse_word(char **lexeme, t_parse_str *cmd_line)
{
	*lexeme = get_substr_until(cmd_line, " <>|\"\'");
	if (!*lexeme)
	{
		return (DEADLY_ERR);
	}
	return (NO_ERR);
}
