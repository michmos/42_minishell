/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:05:11 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/19 17:17:57 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


t_error	parsing(t_list **cmd_lst, char *str, t_list *env_lst)
{
	t_parse_str	cmd_line;
	t_error		error;
	t_list		*token_lst;

	cmd_line = (t_parse_str) { str, ft_strlen(str) };

	// lexer
	token_lst = NULL;
	error = create_token_lst(&token_lst, &cmd_line);

	// parser
	if (!error)
	{
		error = create_cmd_lst(cmd_lst, &token_lst, env_lst);
	}

	// error handling
	if (error)
	{
		if (token_lst)
			ft_lstclear(&token_lst, free_token);
		if (*cmd_lst)
			ft_lstclear(cmd_lst, free_token);
		return (error);
	}
	return (NO_ERR);
}
