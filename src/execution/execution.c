/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/30 15:05:30 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/29 12:04:11 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_error	execution(t_list *pars_out)
{
	int			stat;
	int			num_cmd;
	t_error		error;

	num_cmd = ft_lstsize(pars_out);
	if (num_cmd == 0)
		return (ERROR);
	if (num_cmd == 1 && get_builtin_type(get_cmd(pars_out)->args[0]) != NO_BUILTIN)
	{
		error = exec_one_builtin(get_cmd(pars_out));
	}
	else
	{
		error = cmd_pipeline(pars_out);
	}
	return (error);
}
