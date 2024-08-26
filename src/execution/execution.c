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

t_cmd_data	*get_cmd_data(t_cmd *cmd)
{
	t_cmd_data	*cmd_data;

	cmd_data = (t_cmd_data*)malloc(sizeof(t_cmd_data));
	if (!cmd_data)
	{
		perror("malloc");
		return (NULL);
	}
	cmd_data->pars_out = cmd;
	cmd_data->builtin = check_builtins(cmd);
	return (cmd_data);
}

int	execution(t_list *pars_out, t_info *info, char *line)
{
	int			stat;

	info->num_cmd = ft_lstsize(pars_out);
	if (info->num_cmd == 0)
		return (ERROR);
	if (info->num_cmd == 1 && check_builtins(get_cmd(pars_out)) > 0)
		info->error = exec_one_builtin(get_cmd(pars_out), line, info);
	else
	{
		stat = cmd_pipeline(pars_out, info, line);
		info->error = WEXITSTATUS(stat);
	}
	return (info->error);
}
