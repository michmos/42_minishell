/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/21 13:30:14 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/11 10:37:11 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	pop_token(t_list **token_lst)
{
	t_token	token;
	t_list	*next_node;

	next_node = (*token_lst)->next;
	token = *((t_token *)(*token_lst)->as_ptr);
	free(((t_token *)(*token_lst)->as_ptr));
	free(*token_lst);
	*token_lst = next_node;
	return (token);
}

void	consume_token(t_list **rem_tokens)
{
	t_token	token;

	if (!rem_tokens || !*rem_tokens)
		return ;
	token = pop_token(rem_tokens);
	free(token.lexeme);
}

t_tag	get_token_tag(t_list *tokens)
{
	if (!tokens)
		return (-1);
	return (((t_token *)(tokens->as_ptr))->tag);
}

bool	is_literal(t_tag tag)
{
	return (tag == WORD || tag == SI_QUO || tag == DU_QUO);
}

bool	is_redir(t_tag tag)
{
	return (tag == I_RD || tag == I_RD_HD || tag == O_RD || tag == O_RD_APP);
}
