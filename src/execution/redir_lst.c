/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_lst.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 13:55:02 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/18 09:54:18 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	in_file(t_cmd *cmd)
{
	t_cmd	*temp;
	int		result;
	int		i;

	temp = cmd;
	i = 0;
	result = -1;
	while (temp->redir_lst)
	{
		if (((t_redir *)(temp->redir_lst))->type == I_RD \
		|| ((t_redir *)(temp->redir_lst))->type == I_RD_HD)
			result = i;
		i++;
		temp->redir_lst = temp->redir_lst->next;
	}
	return (result);
}

int	out_file(t_cmd *cmd)
{
	t_cmd	*temp;
	int		result;
	int		i;

	temp = cmd;
	i = 0;
	result = -1;
	while (temp->redir_lst)
	{
		if (((t_redir *)(temp->redir_lst))->type == O_RD \
		|| ((t_redir *)(temp->redir_lst))->type == O_RD_APP)
			result = i;
		i++;
		temp->redir_lst = temp->redir_lst->next;
	}
	return (result);
}

void	check_last_in_out(t_cmd_data *cmd)
{
	int			i;
	t_cmd_data	*temp;

	cmd->last_input = -1;
	cmd->last_output = -1;
	i = 0;
	temp = cmd;
	cmd->redir_count = ft_lstsize(temp->pars_out->redir_lst);
	cmd->hd_count = 0;
	while (i < cmd->redir_count)
	{
		check_type(cmd, ((t_redir *)(temp->pars_out->redir_lst))->type, i);
		temp->pars_out->redir_lst = (temp->pars_out->redir_lst)->next;
		i++;
	}
}

void	check_type(t_cmd_data *cmd, t_tag type, int i)
{
	if (type == I_RD)
		cmd->last_input = i;
	else if (type == O_RD)
		cmd->last_output = i;
	else if (type == O_RD_APP)
		cmd->last_output = i;
	else if (type == I_RD_HD)
	{
		cmd->last_output = i;
		cmd->hd_count++;
	}
}
