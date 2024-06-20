/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extend_redir_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:47:10 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/19 14:46:49 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// TODO: clear up this function
static t_error	parse_redir(t_redir **redir, t_list **rem_tokens)
{
	t_token	token;
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (SYS_ERR);

	// get tag
	token = pop_token(rem_tokens);
	new_redir->type = token.tag;
	free(token.lexeme);

	// check next token
	if (!*rem_tokens)
	{
		free(new_redir);
		printf("Syntax error\n");
		return (SYN_ERR);
	}
	else if (get_token_tag(*rem_tokens)== WHITESPACE)
		consume_token(rem_tokens);

	// get filename
	if (*rem_tokens && get_token_tag(*rem_tokens)== WORD)
	{
		token = pop_token(rem_tokens);
		new_redir->filename = token.lexeme;
	}
	else
	{
		free(new_redir);
		printf("Syntax error\n");
		return (SYN_ERR);
	}
	*redir = new_redir;
	return (NO_ERR);
}

t_error	extend_redir_lst(t_list **head, t_list **rem_tokens)
{
	t_list  *new;
	t_redir *redir;
	t_error	error;

	redir = NULL;
	error = parse_redir(&redir, rem_tokens);
	if (error)
	{
		return (error);
	}
	new = ft_lstnew(redir);
	if (!new)
	{
		free_redir(redir);
		return (SYS_ERR);
	}
	ft_lstadd_back(head, new);
	return (NO_ERR);
}
