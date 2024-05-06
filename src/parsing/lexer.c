/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:15:08 by mmoser            #+#    #+#             */
/*   Updated: 2024/05/06 13:47:10 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_token(void *token)
{
	free(((t_token *)token)->lexeme);
}

static t_tag	get_tag(t_parse_str *cmd_line)
{
	if (cur_char(cmd_line) == '>')
	{
		if (peek_char(cmd_line) == '>')
		{
			return (O_RD_APP);
		}
		return (O_RD);
	}
	else if (cur_char(cmd_line) == '<')
	{
		if (peek_char(cmd_line) == '<')
		{
			return (I_RD_HD);
		}
		return (I_RD);
	}
	else if (cur_char(cmd_line) == '|')
	{
		return (PIPE);
	}
	else if (cur_char(cmd_line) == '"')
	{
		return (DU_QUO);
	}
	else if (cur_char(cmd_line) == 39)
	{
		return (SI_QUO);
	}
	else
	{
		return (WORD);
	}
}

static char	*get_lexeme(t_parse_str *cmd_line, t_tag tag)
{
	t_vec	vec;
	char	*lexeme;

	vec.size = VEC_START_SZ;
	vec.buf = ft_calloc(sizeof(char), vec.size);
	if (!vec.buf)
	{
		return(NULL);
	}

	if (tag == SI_QUO || tag == DU_QUO)
	{
		if(add_str_literal(cmd_line, &vec) != NO_ERR)
		{
			free(vec.buf);
			return (NULL);
		}
	}
	else if (tag == WORD)
	{
		if(add_word(cmd_line, &vec) != NO_ERR)
		{
			free(vec.buf);
			return (NULL);
		}
	}
	else
	{
		if (tag == I_RD_HD || tag == O_RD_APP)
			next_char(cmd_line);
		vec.buf[0] = '\0';
	}
	lexeme = ft_realloc(vec.buf, ft_strlen(vec.buf) + 1, vec.size);
	return (lexeme);
}

static t_token	*get_token(t_parse_str *cmd_line)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		return (NULL);
	}

	new->tag = get_tag(cmd_line);
	new->lexeme = get_lexeme(cmd_line, new->tag);
	if (!new->lexeme)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

t_list	*lexer(t_parse_str *cmd_line)
{
	t_list	*head;
	t_list	*new;
	t_token	*token;

	head = NULL;
	while(cmd_line->cursor_pos < cmd_line->buf_len)
	{
		if (!is_whitespace(cur_char(cmd_line)))
		{
			token = get_token(cmd_line);
			if (!token)
			{
				ft_lstclear(&head, free_token);
				return (NULL);
			}
			new = ft_lstnew(token);
			if (!new)
			{
				ft_lstclear(&head, free_token);
				return (NULL);
			}
			ft_lstadd_back(&head, new);
		}
		next_char(cmd_line);
	}
	return (head);
}
