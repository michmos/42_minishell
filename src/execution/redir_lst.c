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
//
// int	get_in_idx(t_list *redir_lst)
// {
// 	int		result;
// 	int		i;
//
// 	i = 0;
// 	result = -1;
// 	while (redir_lst)
// 	{
// 		if(get_redir(redir_lst)->type == I_RD
// 			|| get_redir(redir_lst)->type == I_RD_HD)
// 			result = i;
// 		i++;
// 		redir_lst= redir_lst->next;
// 	}
// 	return (result);
// }
//
// int	get_out_idx(t_list *redir_lst)
// {
// 	int		result;
// 	int		i;
//
// 	i = 0;
// 	result = -1;
// 	while (redir_lst)
// 	{
// 		if (((t_redir *)(redir_lst->as_ptr))->type == O_RD \
// 		|| ((t_redir *)(redir_lst->as_ptr))->type == O_RD_APP)
// 			result = i;
// 		i++;
// 		redir_lst = redir_lst->next;
// 	}
// 	return (result);
// }

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
