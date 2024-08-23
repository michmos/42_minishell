/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/30 15:05:30 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/23 14:56:17 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execution(t_list *head, t_info *info, char *line)
{
	t_list		*temp;
	t_cmd		*cmd;
	t_cmd_data	*cmd_lst;
	int			stat;

	temp = head;
	cmd = NULL;
	info->num_cmd = ft_lstsize(temp);
	if (info->num_cmd == 0)
		return (ERROR);
	cmd_lst = (t_cmd_data*)malloc(sizeof(t_cmd_data));
	if (!cmd_lst)
		return (free_info(info), 1);
	cmd = get_cmd(head);
	cmd_lst->pars_out = cmd;
	cmd_lst->builtin = check_builtins(head, cmd);
	if (info->num_cmd == 1 && cmd_lst->builtin > 0)
		info->error = exec_one_builtin(cmd_lst, line, info);
	else
	{
		stat = cmd_pipeline(head, info, line);
		info->error = WEXITSTATUS(stat);
	}
	return (free_cmd_lst((void *)cmd_lst), info->error);
}
