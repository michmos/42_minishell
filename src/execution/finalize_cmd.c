/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   finalize_cmd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:16:18 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/17 11:16:39 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	finalize_cmd(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_cmd)
	{
        /*
            check for redirection doubles
            populate last in and out with check_last_in_out
            if cmd->hd_count > 0
                init_heredoc() we read from stdin i guess??
            we can check if cmd is builtin
        */
	}
}
