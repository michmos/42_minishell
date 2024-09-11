/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/07 11:33:33 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/11 10:49:40 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_error	parse_pipe(t_list **rem_tokens)
{
	if (get_token_tag(*rem_tokens) != PIPE)
	{
		ft_printf_fd(STDERR_FILENO, "syntax error: unexpected token: %s\n", \
		((t_token *)((*rem_tokens)->as_ptr))->lexeme);
		return (ERR);
	}
	consume_token(rem_tokens);
	if (!*rem_tokens)
	{
		ft_printf_fd(STDERR_FILENO, "syntax error: unexpected token: %s\n", \
		"new line");
		return (ERR);
	}
	return (NO_ERR);
}

// TODO: only do normconform on very end
static t_error	parse_cmd(t_cmd **cmd, t_list **rem_tokens)
{
	t_error	error;
	t_list	*arg_lst;
	t_cmd	*new_cmd;

	new_cmd = ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	error = NO_ERR;
	arg_lst = NULL;
	while (!error && *rem_tokens && get_token_tag(*rem_tokens) != PIPE)
	{
		if (get_token_tag(*rem_tokens) == WHITESPACE)
		{
			consume_token(rem_tokens);
		}
		else if (is_redir(get_token_tag(*rem_tokens)))
		{
			error = extend_redir_lst(&new_cmd->redir_lst, rem_tokens);
		}
		else if (is_literal(get_token_tag(*rem_tokens)))
		{
			error = extend_arg_lst(&arg_lst, rem_tokens);
		}
	}
	if (!error)
	{
		new_cmd->args = store_ptrs_in_arr(arg_lst);
		if (!new_cmd->args)
		{
			perror("malloc");
			error = DEADLY_ERR;
		}
	}
	if (error)
	{
		ft_lstclear(&arg_lst, free);
		free_cmd(new_cmd);
		return (error);
	}
	ft_lstclear(&arg_lst, NULL);
	*cmd = new_cmd;
	return (NO_ERR);
}

static t_error	extend_cmd_lst(t_list **cmd_lst, t_list **rem_tokens)
{
	t_error	error;
	t_cmd	*cmd;
	t_list	*new;

	cmd = NULL;
	if (get_token_tag(*rem_tokens) == PIPE)
	{
		ft_printf_fd(STDERR_FILENO, "syntax error: unexpected token: %s\n", \
		"pipe");
		return (ERR);
	}
	error = parse_cmd(&cmd, rem_tokens);
	if (error)
	{
		return (error);
	}
	new = ft_lstnew(cmd);
	if (!new)
	{
		perror("malloc");
		free_cmd(cmd);
		return (DEADLY_ERR);
	}
	ft_lstadd_back(cmd_lst, new);
	return (NO_ERR);
}

t_error	create_cmd_lst(t_list **cmd_lst, t_list **token_lst)
{
	t_error	error;

	*cmd_lst = NULL;
	error = NO_ERR;
	while (*token_lst && !error)
	{
		error = extend_cmd_lst(cmd_lst, token_lst);
		if (*token_lst && !error)
		{
			error = parse_pipe(token_lst);
		}
	}
	if (error)
	{
		ft_lstclear(cmd_lst, free_cmd);
		return (error);
	}
	return (NO_ERR);
}
