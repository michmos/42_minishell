/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_lst.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 13:55:02 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/11 09:48:21 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int in_file(t_cmd *cmd)
{
	t_cmd *temp;
	int result;
	int i;

	temp = cmd;
	i = 0;
	result = -1;
	while (temp->redir_lst)
	{
		if (((t_redir *)(temp->redir_lst))->type == I_RD || ((t_redir *)(temp->redir_lst))->type == I_RD_HD)
			result = i;
		i++;
		temp->redir_lst = temp->redir_lst->next;
	}
	return (result);
}

int out_file(t_cmd *cmd)
{
	t_cmd *temp;
	int result;
	int i;

	temp = cmd;
	i = 0;
	result = -1;
	while (temp->redir_lst)
	{
		if (((t_redir *)(temp->redir_lst))->type == O_RD || ((t_redir *)(temp->redir_lst))->type == O_RD_APP)
			result = i;
		i++;
		temp->redir_lst = temp->redir_lst->next;
	}
	return (result);
}

void check_last_in_out(t_cmd *cmd)
{
	int i;

	cmd->last_input = -1;
	cmd->last_output = -1;
	i = 0;
	cmd->redir_count = ft_lstsize(cmd->redir_lst);
	cmd->hd_count = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redir_lst[i]->type == I_RD)
			cmd->last_input = i;
		else if (cmd->redir_lst[i]->type == O_RD)
			cmd->last_output = i;
		else if (cmd->redir_lst[i]->type == O_RD_APP)
			cmd->last_output = i;
		else if (cmd->redir_lst[i]->type == I_RD_HD)
		{
			cmd->last_input = i;
			cmd->hd_count++;
		}
		i++;
	}
}