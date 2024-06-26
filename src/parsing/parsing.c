/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:05:11 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/24 15:18:54 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_lists(t_list **token_lst, t_list **cmd_lst)
{
	if (token_lst && *token_lst)
	{
		ft_lstclear(token_lst, free_token);
	}
	if (cmd_lst && *cmd_lst)
	{
		ft_lstclear(cmd_lst, free_cmd);
	}
}


t_error	parsing(t_list **cmd_lst, char *str, t_list *env_lst)
{
	t_parse_str	cmd_line;
	t_list		*token_lst;
	t_error		error;

	cmd_line = (t_parse_str) { .buf=str, .buf_len=ft_strlen(str) };
	token_lst = NULL;

	// lexer
	error = create_token_lst(&token_lst, &cmd_line);
	if (error)
	{
		free_lists(&token_lst, NULL);
		return (error);
	}

	// parser
	error = create_cmd_lst(cmd_lst, &token_lst, env_lst);
	if (error)
	{
		free_lists(&token_lst, cmd_lst);
		return (error);
	}

	return (NO_ERR);
}
