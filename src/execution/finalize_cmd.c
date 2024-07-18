/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   finalize_cmd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:16:18 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/18 09:34:49 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	finalize_cmd(t_info *info, t_cmd_data *cmd)
{
	int	i;

	i = 0;
	while (i < info->num_cmd)
	{
		check_last_in_out(cmd);
		if (cmd->hd_count > 0)
			init_heredoc(cmd, info);
		i++;
	}
}

        /*
            we can check if cmd is builtin
        */