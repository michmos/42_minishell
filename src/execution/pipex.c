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
#include <unistd.h>

static t_error	child_process(t_cmd *cmd, t_info *info, int child_i, char *line)
{
	t_cmd_data	*cmd_data;

	cmd_data = get_cmd_data(cmd);
	if (!cmd_data)
	{
		return (SYS_ERR);
	}
	if (cmd_data->builtin > 0)
		execute_builtin(cmd_data, line, info);
	info->our_env = converter(info->env_lst);
	cmd_data->cmd_path = find_command_path(cmd_data->pars_out->args[0], info->our_env);
	check_dir(cmd_data, info);
	check_cmd(cmd_data, info);
	return (NO_ERR);
}

void	wait_for_childs(pid_t last_child, int *status)
{
	waitpid(last_child, status, 0);  // TODO: protection required?
	while (wait(NULL) != -1)
		;
}

// parent
t_error	close_unused_fds(int pipe[2], int last_rd_end, size_t i, size_t num_childs)
{
	if (i != num_childs - 1)
	{
		if (close(pipe[1]) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}

	}
	if (i != 0)
	{
		if (close(last_rd_end) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}
	}
	return (NO_ERR);
}

// t_error	set_io(t_list *redir_lst, int last_rd_end, int fds[2])
// {
// 	int	in_fd;
// 	int	out_fd;
//
//
// 	in_fd = in_file(redir_lst);
// 	out_fd = out_file(redir_lst);
//
//
// }

t_error	set_io_pipes(int pipe[2], int last_rd_end, int child_i, size_t num_childs)
{
	if (child_i > 0)
	{
		if (dup2(last_rd_end, STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (SYS_ERR);
		}
		if (close(last_rd_end) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}
	}
	if (child_i < num_childs -1)
	{
		// redirect stdout to pipe
		if (dup2(pipe[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return (SYS_ERR);
		}
		if (close(pipe[1]) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}
		// close unused read end
		if (close(pipe[0]) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}
	}
	return (NO_ERR);
}

int	cmd_pipeline(t_list *cmd_lst, t_info *info, char *line)
{
	int	i;
	int	status;
	pid_t	pid;
	int		fds[2];
	int		last_rd_end;
	char	*hd_str;

	i = 0;
	while (i < info->num_cmd)
	{
		if (pipe(fds) == -1)
			wait_free_exit(cmd_lst, EXIT_FAILURE);
		// execute heredocs
		if (exec_hd(&hd_str, get_cmd(cmd_lst)->redir_lst) != NO_ERR)
			return (SYS_ERR);
		pid = fork();
		if (pid == ERROR)
			wait_free_exit(cmd_lst, EXIT_FAILURE);
		else if (pid == 0)
		{
			// set in out
			if (set_io_pipes(fds,	last_rd_end, i, info->num_cmd) != NO_ERR)
				; // TODO: protect
			if (set_io_redirs(get_cmd(cmd_lst)->redir_lst, hd_str) != NO_ERR)
				; // TODO: protect

			signal(SIGQUIT, sigquit_handle);
			child_process((t_cmd *)(cmd_lst->as_ptr), info, i, line); // TODO: protect
			// free head and move to next node, good representation is in lstclear in while loop
		}
		// parent
		if (close_unused_fds(fds, last_rd_end, i, info->num_cmd) != NO_ERR)
		{
			; // TODO: protect
		}
		last_rd_end = fds[0];
		cmd_lst = cmd_lst->next;
		i++;
	}
	wait_for_childs(pid, &status);
	return (status);
}
