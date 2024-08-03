/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/30 15:05:30 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/17 13:42:52 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int execution(t_list *head, t_info *info, char *line)
{
	t_list *temp;
	t_cmd_data *cmd;
	int stat;

	temp = head;
	info->num_cmd = ft_lstsize(temp);
	if (info->num_cmd == 0)
		return (ERROR);
	cmd->builtin = check_builtins(temp);
	if (info->num_cmd == 1 && cmd->builtin > 0)
		info->error = exec_one_builtin(head, line, info);
	else
	{
		stat = cmd_pipeline(head, info);
		info->error = WEXITSTATUS(stat);
	}
	return (info->error);
}
