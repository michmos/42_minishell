/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:33:33 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/19 17:16:58 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_error	parse_pipe(t_list **rem_tokens)
{
	if (get_token_tag(*rem_tokens) != PIPE)
	{
		printf("Syntax Error\n");
		return (SYN_ERR);
	}
	consume_token(rem_tokens);
	if (!*rem_tokens)
	{
		printf("Syntax Error\n");
		return (SYN_ERR);
	}
	return (NO_ERR);
}

// TODO: clear up this function
static t_error	parse_cmd(t_cmd **cmd, t_list **rem_tokens, t_list *env_lst)
{
	t_error	error;
	t_list	*arg_lst;
	t_cmd	*new_cmd;

	error = NO_ERR;

	new_cmd = ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
		return (SYS_ERR);

	if (get_token_tag(*rem_tokens) == PIPE)
	{
		free(new_cmd);
		printf("Syntax error");
		return (SYN_ERR);
	}

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
			error = extend_arg_lst(&arg_lst, rem_tokens, env_lst);
		}
	}
	
	if (!error)
	{
		new_cmd->args = store_ptrs_in_arr(arg_lst);
		if (!new_cmd->args)
			error = SYS_ERR;
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

static t_error	extend_cmd_lst(t_list **cmd_lst, t_list **rem_tokens, t_list *env_lst)
{
	t_error	error;
	t_cmd	*cmd;
	t_list	*new;

	cmd = NULL;
	error = parse_cmd(&cmd, rem_tokens, env_lst);
	if (error)
	{
		return (error);
	}
	new = ft_lstnew(cmd);
	if (!new)
	{
		free_cmd(cmd);
		return (SYS_ERR);
	}
	ft_lstadd_back(cmd_lst, new);
	return (NO_ERR);
}

t_error	create_cmd_lst(t_list **cmd_lst, t_list **token_lst, t_list *env_lst)
{
	t_error	error;

	*cmd_lst = NULL;
	error = NO_ERR;
	while (*token_lst && !error)
	{
		error = extend_cmd_lst(cmd_lst, token_lst, env_lst);
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
