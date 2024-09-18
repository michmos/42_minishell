/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 17:15:08 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/18 11:31:44 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_tag	get_tag(t_parse_str *cmd_line)
{
	if (ft_is_whitespace(cur_char(cmd_line)))
		return (WHITESPACE);
	else if (cur_char(cmd_line) == '>')
	{
		if (peek_char(cmd_line) == '>')
			return (O_RD_APP);
		return (O_RD);
	}
	else if (cur_char(cmd_line) == '<')
	{
		if (peek_char(cmd_line) == '<')
			return (I_RD_HD);
		return (I_RD);
	}
	else if (cur_char(cmd_line) == '|')
		return (PIPE);
	else if (cur_char(cmd_line) == '"')
		return (DU_QUO);
	else if (cur_char(cmd_line) == '\'')
		return (SI_QUO);
	else
		return (WORD);
}

static t_error	init_lexeme(char **lexeme, t_parse_str *cmd_line, t_tag tag)
{
	t_error	error;

	error = NO_ERR;
	if (tag == WHITESPACE)
	{
		while (cur_char(cmd_line) && ft_is_whitespace(cur_char(cmd_line)))
			advance_char(cmd_line);
	}
	else if (tag == I_RD || tag == O_RD || tag == PIPE)
		advance_char(cmd_line);
	else if (tag == I_RD_HD || tag == O_RD_APP)
	{
		advance_char(cmd_line);
		advance_char(cmd_line);
	}
	else if (tag == WORD)
		error = parse_word(lexeme, cmd_line);
	else if (tag == SI_QUO)
		error = parse_str_sq(lexeme, cmd_line);
	else if (tag == DU_QUO)
		error = parse_str_dq(lexeme, cmd_line);
	return (error);
}

static t_error	create_token(t_token **token, t_parse_str *cmd_line)
{
	t_error	error;
	t_token	*new_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	new_token->tag = get_tag(cmd_line);
	error = init_lexeme(&new_token->lexeme, cmd_line, new_token->tag);
	if (error)
	{
		free_token(new_token);
		return (error);
	}
	*token = new_token;
	return (NO_ERR);
}

t_error	create_token_lst(t_list **head, char *str)
{
	t_list		*new;
	t_token		*token;
	t_error		error;
	t_parse_str	cmd_line;

	cmd_line = (t_parse_str){.buf = str, .buf_len = ft_strlen(str)};
	while (cmd_line.cursor_pos < cmd_line.buf_len)
	{
		error = create_token(&token, &cmd_line);
		if (error)
		{
			ft_lstclear(head, free_token);
			return (error);
		}
		new = ft_lstnew(token);
		if (!new)
		{
			perror("malloc");
			free_token(token);
			ft_lstclear(head, free_token);
			return (DEADLY_ERR);
		}
		ft_lstadd_back(head, new);
	}
	return (NO_ERR);
}
