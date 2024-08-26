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

int	execution(t_list *pars_out, t_info *info, char *line)
{
	t_cmd		*frst_cmd;
	t_cmd_data	*cmd_data;
	int			stat;

	frst_cmd = NULL;
	info->num_cmd = ft_lstsize(pars_out);
	if (info->num_cmd == 0)
		return (ERROR);
	cmd_data = (t_cmd_data*)malloc(sizeof(t_cmd_data));
	if (!cmd_data)
		return (free_info(info), 1);
	frst_cmd = get_cmd(pars_out);
	cmd_data->pars_out = frst_cmd;
	cmd_data->builtin = check_builtins(pars_out, frst_cmd);
	if (info->num_cmd == 1 && cmd_data->builtin > 0)
		info->error = exec_one_builtin(cmd_data, line, info);
	else
	{
		stat = cmd_pipeline(pars_out, info, line);
		info->error = WEXITSTATUS(stat);
	}
	return (free_cmd_lst((void *)cmd_data), info->error);
}
