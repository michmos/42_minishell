/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   extend_redir_lst.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/23 15:47:10 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/11 10:50:28 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*get_redir_filename(t_list **rem_tokens)
{
	t_token	token;
	char	*filename;
	char	*temp;

	filename = NULL;
	while (*rem_tokens && is_literal(get_token_tag(*rem_tokens)))
	{
		token = pop_token(rem_tokens);
		temp = ft_strjoin(filename, token.lexeme);
		free(filename);
		free(token.lexeme);
		if (!temp)
		{
			return (NULL);
		}
		filename = temp;
	}
	return (filename);
}

static t_tag	get_redir_type(t_list **rem_tokens)
{
	t_token	token;

	token = pop_token(rem_tokens);
	free(token.lexeme);
	return (token.tag);
}

static t_error	parse_redir(t_redir **redir, t_list **rem_tokens)
{
	t_redir	new_redir;

	new_redir.type = get_redir_type(rem_tokens);

	// check next token
	if (!*rem_tokens)
	{
		ft_printf_fd(STDERR_FILENO, "syntax error: missing redirection target\n");
		return (ERR);
	}

	if (get_token_tag(*rem_tokens)== WHITESPACE)
	{
		consume_token(rem_tokens);
	}

	// check next token
	if (!*rem_tokens || !is_literal(get_token_tag(*rem_tokens)))
	{
		ft_printf_fd(STDERR_FILENO, "syntax error: missing redirection target\n");
		return (ERR);
	}

	new_redir.filename = get_redir_filename(rem_tokens);
	if (!new_redir.filename)
	{
		return (DEADLY_ERR);
	}

	*redir = ft_calloc(1, sizeof(t_redir));
	if (!*redir)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	**redir = new_redir;
	return (NO_ERR);
}

t_error	extend_redir_lst(t_list **head, t_list **rem_tokens)
{
	t_list	*new;
	t_redir	*redir;
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
		perror("malloc");
		return (DEADLY_ERR);
	}
	ft_lstadd_back(head, new);
	return (NO_ERR);
}
