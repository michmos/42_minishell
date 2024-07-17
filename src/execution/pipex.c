/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 09:50:20 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/17 14:05:35 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_process(t_list *head, t_info *info, int child_i)
{
	t_cmd_data	*cmd;

	close(info->std_in);
	close(info->std_out);
	cmd = get_cmd(head);
	cmd->last_input = in_file(cmd->pars_out);
	cmd->last_output = out_file(cmd->pars_out);
	open_files(cmd, info);
	pipe_cmd(cmd, info, child_i);
	close_fd_array(cmd, info);
	close_pipes(info);
	if (cmd->builtin > 0)
		execute_builtin(cmd);
	cmd->cmd_path = find_command_path(cmd->pars_out->args[0], info->our_env);
	check_dir(cmd, info);
	check_cmd(cmd, info);
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

int	cmd_pipeline(t_list *head, t_info *info)
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
