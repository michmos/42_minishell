/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:11:45 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/20 14:23:29 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_redir(void	*redir)
{
	t_redir *ptr;

	ptr = (t_redir *) redir;
	free(ptr->filename);
	free(ptr);
}

void	free_cmd(void *cmd)
{
	t_cmd	*ptr;

	ptr = (t_cmd *) cmd;
	ft_free_2d_array((void **)(ptr->args));
	ft_lstclear(&ptr->redir_lst, free_redir);
	free(cmd);
}

void	free_token(void *token)
{
	free(((t_token *)token)->lexeme);
}
