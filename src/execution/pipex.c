/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 09:50:20 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/11 11:42:25 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_pipes(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_cmd - 1)
	{
		close(info->fd[i][0]);
		close(info->fd[i][1]);
		i++;
	}
}

void	child_process(t_list *head, t_info *info, int child_i)
{
	t_cmd	*cmd;

	cmd = get_cmd(head);
	open_files(cmd, info);
	cmd->last_input = in_file(cmd);
	cmd->last_output = out_file(cmd);
	pipe_cmd(cmd, info, child_i);
	close_fd_array(cmd, info);
	close_pipes(info);
	execute_builtin(cmd->builtin);
	check_dir();
	check_cmd();
}

int	parent_process(t_info *info)
{
	int	i;
	int	status;

	close_pipes(info);
	i = 0;
	while (i < info->num_cmd)
	{
		waitpid(info->pid[i], &status, 0);
		i++;
	}
	return (status);
}

int	cmd_pipeline(t_list *head, t_info *info, char **env)
{
	int	i;
	int	status;

	i = -1;
	while (++i < info->num_cmd - 1)
		if (pipe(info->fd[i]) == -1)
			wait_free_exit((head, EXIT_FAILURE));
	i = -1;
	while (++i < info->num_cmd)
	{
		info->pid[i] = fork();
		if (info->pid[i] == ERROR)
			wait_free_exit(head, EXIT_FAILURE);
		else if (info->pid[i] == 0)
		{
			child_process(head, info, i);
			// free head and move to next node
		}
	}
	status = parent_process(info); // waits for all child processes to finish
	return (status);			   // could be used as error code message
}
// free the node after use,
// maybe write a function that takes head, frees it and returns head->next
