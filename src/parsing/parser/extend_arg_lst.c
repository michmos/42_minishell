/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extend_arg_lst.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:47:41 by mmoser            #+#    #+#             */
/*   Updated: 2024/08/06 15:37:57 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_arg(t_list **rem_tokens)
{
	t_token	token;
	char	*arg;
	char	*temp;

	temp = NULL;
	arg = NULL;
	while (*rem_tokens && is_literal(get_token_tag(*rem_tokens)))
	{
		token = pop_token(rem_tokens);
		if (token.tag == WORD || token.tag == DU_QUO)
		{
			if (expand_all_env_vars(&token.lexeme) != NO_ERR)
			{
				free(token.lexeme);
				free(arg);
				return (NULL);
			}
		}
		temp = ft_strjoin(arg, token.lexeme);
		free(token.lexeme);
		free(arg);
		if (!temp)
		{
			perror("malloc");
			return (NULL);
		}
		arg = temp;
	}
	return (arg);
}

t_error	extend_arg_lst(t_list **args_lst, t_list **rem_tokens)
{
	t_list	*new;
	char	*arg;

	arg = get_arg(rem_tokens);
	if (!arg)
	{
		return (SYS_ERR);
	}
	new = ft_lstnew(arg);
	if (!new)
	{
		free(arg);
		perror("malloc");
		return (SYS_ERR);
	}
	ft_lstadd_back(args_lst, new);
	return (NO_ERR);
}
