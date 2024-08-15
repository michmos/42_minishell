/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/30 15:05:30 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/15 14:25:04 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execution(t_list *head, t_info *info, char *line)
{
	t_list		*temp;
	t_cmd_data	*cmd;
	int			stat;

	temp = head;
	info->num_cmd = ft_lstsize(temp);
	if (info->num_cmd == 0)
		return (ERROR);
	cmd = malloc(sizeof(t_cmd_data));
	if (!cmd)
	{
		// need to exit and clean everything
	}
	cmd = get_cmd(head);
	cmd->builtin = check_builtins(head, cmd->pars_out);
	if (info->num_cmd == 1 && cmd->builtin > 0)
		info->error = exec_one_builtin(head, line, info);
	else
	{
		stat = cmd_pipeline(head, info, line);
		info->error = WEXITSTATUS(stat);
	}
	free_cmd_lst((void *)cmd);
	return (info->error);
}

/*
talk to micha cause i'm completely lost why i can not access args[0]
when i try running minishell with env
*/