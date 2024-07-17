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

void	pipex(t_list *head, t_info *info, char *line)
{
	t_list		*temp;
	t_cmd_data	*cmd;
	int			stat;

	temp = head;
	info->num_cmd = ft_lstsize(temp);
	if (info->num_cmd == 0)
		return (ERROR);
	cmd->builtin = check_builtins(temp);
	if (info->num_cmd == 1 && cmd->builtin > 0)
		info->error = exec_one_builtin(head, line, info);
	else
		stat = cmd_pipeline(head, info);
}
/*
	with the use of signals manipulate this while loop
	once the prompt is showed we should wait for the user 
	to input something and after that is executed we can
	print the prompt again
*/

/*
	the signal part and prompt part don't make any sense
	i can't seem to find a way to make the prompt on the screen stay
	or it doesn't even show up and the while loop terminates quite fast
	just by itself without typing anything and none of my commands work... 
*/