/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 09:50:20 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/23 14:53:40 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	create_pid_array(t_info *info)
{
	info->pid = malloc(sizeof(int) * info->num_cmd);
}

void	child_process(t_list *head, t_info *info, int child_i, char *line)
{
	t_cmd_data	*cmd;
	t_cmd		*test;

	cmd = malloc(sizeof(t_cmd_data));
	if (!cmd)
		return ;
	close(info->std_in);
	close(info->std_out);
	test = get_cmd(head);
	cmd->pars_out = test;
	cmd->last_input = in_file(cmd->pars_out->redir_lst);
	cmd->last_output = out_file(cmd->pars_out->redir_lst);
	open_files(cmd, CHILD, info);
	pipe_cmd(cmd, info, child_i);
	close_fd_array(cmd, info);
	close_pipes(info);
	if (cmd->builtin > 0)
		execute_builtin(cmd, line, info);
	info->our_env = converter(info->env_lst);
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

int	cmd_pipeline(t_list *head, t_info *info, char *line)
{
	int	i;
	int	status;

	i = -1;
	while (++i < info->num_cmd - 1)
		if (pipe(info->fd[i]) == -1)
			wait_free_exit(head, EXIT_FAILURE);
	i = -1;
	create_pid_array(info);
	while (++i < info->num_cmd)
	{
		info->pid[i] = fork();
		if (info->pid[i] == ERROR)
			wait_free_exit(head, EXIT_FAILURE);
		else if (info->pid[i] == 0)
		{
			signal(SIGQUIT, sigquit_handle);
			child_process(head, info, i, line);
			// free head and move to next node, good representation is in lstclear in while loop
		}
	}
	status = parent_process(info);
	return (status);
}
