/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/30 15:05:30 by pminialg      #+#    #+#                 */
/*   Updated: 2024/06/27 11:46:16 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pipex(t_list *head, char **env)
{
	t_list		*temp;
	pid_t		pid;
	int			num_cmds;
	int			stat_loc;

	temp = head;
	num_cmds = ft_lstsize(temp);
	pid = cmd_pipeline(head, num_cmds, env);
	waitpid(pid, &stat_loc, 0);
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