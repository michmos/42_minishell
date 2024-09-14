/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/24 16:05:11 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/11 10:56:35 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_error	parsing(t_list **cmd_lst, char **str)
{
	t_list		*token_lst;
	t_error		error;

	token_lst = NULL;
	*cmd_lst = NULL;

	// expansion
	error = expand_all_env_vars(str);
	if (error)
	{
		return (error);
	}
	// lexer
	error = create_token_lst(&token_lst, *str);
	if (error)
	{
		return (error);
	}
	// parser
	error = create_cmd_lst(cmd_lst, &token_lst);
	if (error)
	{
		ft_lstclear(&token_lst, free_token);
		return (error);
	}
	return (NO_ERR);
}
